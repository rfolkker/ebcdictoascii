#include "CSVFileImutable.h"
#include "IntegerHelper.h"
#include "PointerRemapper.h"
#include "PointerFunctions.h"


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
void SCSVRowImmutable::Dump(void)
{	
	int			i;
	CString		szString;
	CString		szLeft;
	char*		pszString;

	szString.Init();
	for (i = 0; i < iNumFields; i++)
	{
		pszString = aszFields[i];
		szLeft.Init();
		szLeft.LeftAlign(pszString, ' ', 15);
		szString.Append(szLeft);
		szLeft.Kill();
		if (i != iNumFields-1)
		{
			szString.Append("| ");
		}
	}
	szString.AppendNewLine();
	szString.Kill();
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
int SCSVRowImmutable::IndexOf(char* szString)
{
	int	i;

	for (i = 0; i < iNumFields; i++)
	{
		if (strcmp(szString, aszFields[i]) == 0)
		{
			return i;
		}
	}
	return -1;
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
char* SCSVRowImmutable::Get(int iIndex)
{
	return aszFields[iIndex];
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
void CCSVFileImmutable::Init(char cSeparator, char cDelimiter)
{
	CCSVFile::Init(cSeparator, cDelimiter);
	mapsCSVRows.Init(16384);
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
void CCSVFileImmutable::Kill(void)
{
	int					i;
	SCSVRowImmutable*	psCSVRow;

	for (i = 0; i < mapsCSVRows.NumElements(); i++)
	{
		psCSVRow = (SCSVRowImmutable*)mapsCSVRows.GetPtr(i);
		FreeRow(psCSVRow);
	}
	mapsCSVRows.Kill();

	CCSVFile::Kill();
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
SCSVRowImmutable* CCSVFileImmutable::AllocateRow(char* szText)
{
	int					iLength;
	char				c;
	int					iNumSeparators;
	SCSVRowImmutable*	psCSVRow;
	int					iTotalSize;
	int					iStringStart;
	int					i;
	char*				pszFieldPos;

	iNumSeparators = 0;

	//The complier can optimise this better than you can... leave it alone.
	for (iLength = 0;; iLength++)
	{
		c = szText[iLength];
		
		//Check for the end of the row.
		if ((c == 0) || (c == '\n') || (c == '\r'))
		{
			break;
		}
		else if (c == mcSeparator)
		{
			iNumSeparators++;
		}
	}

	//Allocate the heading size and the first char*, then the rest of the char pointers (iNumSeparators is the number
	//of fields -1), then allocate the string and lastly the terminating zero.
	iStringStart = sizeof(SCSVRowImmutable) + iNumSeparators * sizeof(char*);
	iTotalSize = iStringStart + iLength + 1;
	psCSVRow = (SCSVRowImmutable*)malloc(iTotalSize);

	psCSVRow->iNumFields = iNumSeparators+1;
	psCSVRow->iRowLength = iLength+1;
	psCSVRow->iTotalSize = iTotalSize;

	pszFieldPos = (char*)RemapSinglePointer(psCSVRow, iStringStart);
	memcpy(pszFieldPos, szText, iLength);
	pszFieldPos[iLength] = mcSeparator;

	iNumSeparators = 0;
	psCSVRow->aszFields[0] = pszFieldPos;
	for (i = 0;; i++)
	{
		c = pszFieldPos[i];

		if (c == mcSeparator)
		{
			pszFieldPos[i] = 0;
			iNumSeparators++;
			if (iNumSeparators == psCSVRow->iNumFields)
			{
				break;
			}
			psCSVRow->aszFields[iNumSeparators] = &pszFieldPos[i+1];
		}
	}
	return psCSVRow;
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
void CCSVFileImmutable::FreeRow(SCSVRowImmutable* psCSVRow)
{
	SafeFree(psCSVRow);
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
BOOL CCSVFileImmutable::ReadLine(void)
{
	SCSVRowImmutable*	psCSVRow;
	BOOL				bResult;
	int					iFilePos;

	iFilePos = mcFileExtended.GetFilePos();
	bResult = ReadLine(-1, &psCSVRow);
	if (!bResult)
	{
		return FALSE;
	}
	mapsCSVRows.Add(psCSVRow, iFilePos);

	return TRUE;
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
BOOL CCSVFileImmutable::ReadLine(int iFileOffset, SCSVRowImmutable** ppsCSVRow)
{
	TRISTATE	tResult;
	char		szBuffer[1024*128];

	if (iFileOffset != -1)
	{
		mcFileExtended.Seek(iFileOffset, EFSO_SET);
	}

	tResult = mcFileExtended.ReadLine(szBuffer, 1024*128);
	if (tResult == TRITRUE)
	{
		*ppsCSVRow = AllocateRow(szBuffer);
		return TRUE;
	}
	return FALSE;
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
void CCSVFileImmutable::ReadAllLines(void)
{
	BOOL	bResult;
	int		iRow = 0;

	do
	{
		bResult = ReadLine();
		iRow++;
	} while (bResult);
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
void CCSVFileImmutable::Dump(void)
{
	int					i;
	SCSVRowImmutable*	psCSVRow;

	for (i = 0; i < mapsCSVRows.NumElements(); i++)
	{
		psCSVRow = (SCSVRowImmutable*)mapsCSVRows.GetPtr(i);
		psCSVRow->Dump();
	}
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
int CCSVFileImmutable::NumRows(void)
{
	return mapsCSVRows.NumElements();
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
SCSVRowImmutable* CCSVFileImmutable::Get(int iRowNum)
{
	return (SCSVRowImmutable*)mapsCSVRows.SafeGetPtr(iRowNum);
}

