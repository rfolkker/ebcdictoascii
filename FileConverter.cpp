#include "FileConverter.h"
#include "FieldConverter.h"
#include <iostream>


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
void CFileConverter::Init(CLogger* pcLog)
{
	mpcLog = pcLog;
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
void CFileConverter::Kill(void)
{
	mpcLog = NULL;
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
int CFileConverter::ConvertFile(char* szOutputFile, char* szInputFile, CTableDescription* pcTableDescription, BOOL* bCSVExists)
{
	CFileExtended	cFileInput;
	CFileExtended	cFileOutput;
	int				iFileSize;
	char*				szFile;
	int				iRowLength;
	int				iNumRows;
	int				iPartialRow;
	int				iNumRowsRead;
	int				i;
	char				szOutputRow[1024*1024];
	char*				szInputRow;
	int				iUsed;
	int				iWritten;
	float				fDotInterval;
	float				fTillDot;
	int				iByteBlockSize;
	int				iNumLinesInBlock;
	int				iLineNum;
	int				iNumRowsToRead;
	CString			cMessage;

	*bCSVExists = FALSE;
	cFileInput.Init(EFT_DISK);
	cFileInput.Open(szInputFile, EFM_READ | EFM_BINARY);

	cFileOutput.Init(EFT_DISK);
	cFileOutput.Open(szOutputFile, EFM_WRITE | EFM_BINARY);

	iFileSize = cFileInput.GetFileLength();
	
	if (iFileSize == 0)
	{
		mpcLog->Log("Empty File");
		return 0;
	}

	iRowLength = pcTableDescription->GetRowLength();
	iNumRows = iFileSize / iRowLength;
	iPartialRow = iFileSize % iRowLength;
	
	if (iPartialRow != 0)
	{
		mpcLog->Log("File contains incomplete line");
		return 1;
	}
	
	PrintNumberOfLines(iNumRows);

	fDotInterval = iNumRows / 40.0f;
	
	iByteBlockSize = 64*1024*1024;
	iNumLinesInBlock = iByteBlockSize / iRowLength;
	szFile = (char*)malloc(iByteBlockSize);
	
	fTillDot = 0.0f;
	iLineNum = iNumLinesInBlock;
	for (i = 0; i < iNumRows; i++)
	{
		if (iLineNum == iNumLinesInBlock)
		{
			if ((iNumRows - i) >= iNumLinesInBlock)
			{
				iNumRowsToRead = iNumLinesInBlock;
			}
			else
			{
				iNumRowsToRead = (iNumRows - i);
			}
			iNumRowsRead = cFileInput.Read(szFile, iRowLength, iNumRowsToRead);
			if (iNumRowsRead != iNumRowsToRead)
			{
				mpcLog->Log("Could not read file");
				return 1;
			}
			iLineNum = 0;
		}

		szInputRow = &szFile[iLineNum * iRowLength];
		iUsed = ConvertRow(szOutputRow, szInputRow, pcTableDescription);
		
		if (iUsed != -1)
		{
			//Nothing went wrong, write out the line.
			szOutputRow[iUsed] = '\n';
			iUsed++;
			iWritten = cFileOutput.Write(szOutputRow, iUsed, 1);
		
			if (fTillDot >= fDotInterval)
			{
				fTillDot -= fDotInterval;
				std::cout << "." << std::flush;
			}
			fTillDot += 1.0f;
			iLineNum++;
		}
		else
		{
			//Something bad happend.
			cMessage.Init(" <- failed on ");
			cMessage.Append(iLineNum);
			cMessage.Append("!");
			mpcLog->Log(cMessage.Text());
			cMessage.Kill();
		}
	}

	cMessage.Init("Done");
	mpcLog->Write(cMessage.Text());
	cMessage.Kill();
    
	cFileOutput.Close();
	cFileOutput.Kill();

	cFileInput.Close();
	cFileInput.Kill();

	free(szFile);
	*bCSVExists = TRUE;
	return 0;
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
int CFileConverter::ConvertRow(char* szOutputRow, char* szInputRow, CTableDescription* pcTableDescription)
{
	int							i;
	int							iNumberOfFields;
	CFieldDescription*	pcDesc;	
	char*						szOutputField;
	char*						szInputField;
	int							iUsed;
	int							iTotalUsed;

	iNumberOfFields = pcTableDescription->GetNumberOfFields();
	szOutputField = szOutputRow;
	iTotalUsed = 0;

	for (i = 0; i < iNumberOfFields; i++)
	{
		pcDesc = pcTableDescription->GetField(i);

		szInputField = &szInputRow[pcDesc->miStartPos];
		iUsed = ConvertField(szOutputField, szInputField, pcDesc);
		if (iUsed == -1)
		{
			return -1;
		}
		if (i != iNumberOfFields-1)
		{
			szOutputField[iUsed] = ',';
			iUsed++;
		}
		szOutputField = &szOutputField[iUsed];
		iTotalUsed += iUsed;
	}
	return iTotalUsed;
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
int CFileConverter::ConvertField(char* szOutputField, char* szInputField, CFieldDescription* pcFieldDescription)
{
	CFieldConverter	cFieldConverter;
	int					iBytesUsed;

	cFieldConverter.Init(mpcLog);
	iBytesUsed = cFieldConverter.ConvertField(szOutputField, szInputField, pcFieldDescription);
	cFieldConverter.Kill();
	return iBytesUsed;
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
void CFileConverter::PrintNumberOfLines(int iNumRows)
{
	CString cNumRows;
	CString cAllignedNumRows;
	
	cNumRows.Init();
	cNumRows.Append(iNumRows);
	cAllignedNumRows.Init();
	cAllignedNumRows.LeftAlign(cNumRows, ' ', 8);
	std::cout << cAllignedNumRows.Text() << " " << std::flush;
	cNumRows.Kill();
	cAllignedNumRows.Kill();
}

