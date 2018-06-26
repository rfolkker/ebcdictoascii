#include "FileExtended.h"
#include "IntegerHelper.h"

//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
EDataTypes CFileExtended::PrivateReadDataType(void)
{
	EDataTypes	eType;

	if (!Read(&eType, sizeof(EDataTypes), 1))
	{
		return DT_Error;
	}
	return eType;
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
BOOL CFileExtended::PrivateWriteDataType(EDataTypes eType)
{
	if (!Write(&eType, sizeof(EDataTypes),1 ))
	{
		return FALSE;
	}
	return TRUE;
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
TRISTATE CFileExtended::ReadData(void* pvData, int iDataSize)
{
	EDataTypes	eType;
	int			iReadSize;

	CheckReadType(eType, DT_Data);
	CheckReadData(&iReadSize, sizeof(int));
	if (iDataSize > iReadSize)
	{
		return TRIFALSE;
	}
	CheckReadData(pvData, iDataSize);
	return TRITRUE;
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
BOOL CFileExtended::WriteData(void* pvData, int iDataSize)
{
	CheckWriteType(DT_Data);
	CheckWriteData(&iDataSize, sizeof(int));
	CheckWriteData(pvData, iDataSize);
	return TRUE;
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
TRISTATE CFileExtended::ReadArrayInt(CArrayInt* pcArray)
{
	EDataTypes	eType;
	int			iElementSize;

	CheckReadType(eType, DT_ArrayInt);
	CheckReadData(&iElementSize, sizeof(int));
	if (iElementSize != sizeof(int))
	{
		return TRIFALSE;
	}
	CheckReadData(pcArray, sizeof(CArrayInt));

	if (pcArray->miUsedElements != 0)
	{
		pcArray->InitFromHeader();
		CheckReadData(pcArray->pvArray, pcArray->ByteSize());
	}
	return TRITRUE;
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
BOOL CFileExtended::WriteArrayInt(CArrayInt* pcArray)
{
	int		iElementSize;

	CheckWriteType(DT_ArrayInt);
	iElementSize = sizeof(int);
	CheckWriteData(&iElementSize, sizeof(int));
	CheckWriteData(pcArray, sizeof(CArrayInt));

	if (pcArray->miUsedElements != 0)
	{
		CheckWriteData(pcArray->pvArray, pcArray->ByteSize());
	}
	return TRUE;
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
TRISTATE CFileExtended::ReadString(char* szString)
{
	EDataTypes	eType;
	int		iReadSize;

	CheckReadType(eType, DT_Chars);
	CheckReadData(&iReadSize, sizeof(int));
	CheckReadData(szString, iReadSize);
	return TRITRUE;
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
BOOL CFileExtended::WriteString(char* szString)
{
	int		iStrLen;

	CheckWriteType(DT_Chars);
	iStrLen = (int)strlen(szString)+1;
	CheckWriteData(&iStrLen, sizeof(int));
	CheckWriteData(szString, iStrLen);
	return TRUE;
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
TRISTATE CFileExtended::ReadString(CString* szString)
{
	EDataTypes	eType;

	CheckReadType(eType, DT_String);
	if (WriteArrayTemplate(&szString->mcText))
	{
		return TRITRUE;
	}
	else
	{
		return TRIERROR;
	}
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
BOOL CFileExtended::WriteString(CString* szString)
{
	CheckWriteType(DT_String);
	return WriteArrayTemplate(&szString->mcText);
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
BOOL CFileExtended::WriteArrayUnknown(CArrayUnknown* pcArray)
{
	CheckWriteType(DT_ArrayUnknown);
	CheckWriteData(pcArray, sizeof(CArrayUnknown));

	if (pcArray->miUsedElements != 0)
	{
		CheckWriteData(pcArray->pvArray, pcArray->ByteSize());
	}
	return TRUE;
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
TRISTATE CFileExtended::ReadArrayUnknown(CArrayUnknown* pcArray)
{
	EDataTypes	eType;

	CheckReadType(eType, DT_ArrayUnknown);
	CheckReadData(pcArray, sizeof(CArrayUnknown));

	if (pcArray->miUsedElements != 0)
	{
		pcArray->InitFromHeader();
		CheckReadData(pcArray->pvArray, pcArray->ByteSize());
	}
	return TRITRUE;
}




//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
#define LINE_BUFFER_LENGTH 256
TRISTATE CFileExtended::ReadLine(CString* szString)
{
	char	snz[LINE_BUFFER_LENGTH+1];
	int		iLength;
	BOOL	bDoneAnything;

	bDoneAnything = FALSE;
	for (;;)
	{
		iLength = PrivateReadLine(snz);
		if (iLength == 0)
		{
			if (bDoneAnything)
			{
				return TRITRUE;
			}
			else
			{
				return TRIFALSE;
			}
		}
		snz[iLength] = 0;
		szString->Append(snz);
		if (iLength < LINE_BUFFER_LENGTH)
		{
			return TRITRUE;
		}
		bDoneAnything = TRUE;
	}
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
TRISTATE CFileExtended::ReadLine(char* szBuffer, int iMaxLength)
{
	int		iLength;
	BOOL	bDoneAnything;
	int		iPosition;

	bDoneAnything = FALSE;
	iPosition = 0;
	for (;;)
	{
		iLength = PrivateReadLine(&szBuffer[iPosition]);
		iPosition += iLength;
		if ((iLength > 0) && (iLength < LINE_BUFFER_LENGTH))
		{
			szBuffer[iPosition] = 0;
			return TRITRUE;
		}
		if (iLength == 0)
		{
			if (bDoneAnything)
			{
				szBuffer[iPosition] = 0;
				return TRITRUE;
			}
			else
			{
				return TRIFALSE;
			}
		}
		bDoneAnything = TRUE;
	}
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
int CFileExtended::PrivateReadLine(char* snz)
{
	int		iNewLinePosition;
	int		iBytesRead;
	int		iPosition;

	iPosition = GetFilePos();
	iBytesRead = Read(snz, 1, LINE_BUFFER_LENGTH);
	if (iBytesRead == 0)
	{
		return 0;
	}
	iNewLinePosition = FindFirstByte(snz, '\n', LINE_BUFFER_LENGTH);
	if (iNewLinePosition != -1)
	{
		if (snz[iNewLinePosition-1] == '\r')
		{
			Seek(iPosition + iNewLinePosition + 1, EFSO_SET);
			iNewLinePosition--;
		}
		else
		{
			Seek(iPosition + iNewLinePosition + 1, EFSO_SET);
		}
		return iNewLinePosition;
	}
	else
	{
		return LINE_BUFFER_LENGTH;
	}
}

