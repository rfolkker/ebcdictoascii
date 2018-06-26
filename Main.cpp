#include "CoreLib/CSVFile.h"
#include "CoreLib/TextFile.h"
#include "CoreLib/FileExtended.h"
#include "TableDescriptionsContainer.h"
#include "FileConverter.h"
#include <iostream>
#include "CoreLib/String.h"
#include "CoreLib/Linux.h"
#include "CoreLib/Logger.h"
#include "SQLCommand.h"


void FormatNicely(char* szDir);
BOOL FileExists(char* szFile);


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
int main(int argc, const char* argv[])
{
	CTableDescriptionsContainer	cTableDescContainer;
	CFileConverter			cFileConverter;
	CTableDescription*		pcTableDesc;
	CSQLCommand			cSQLCommand;
	char	 			szFileName[512];
	char	 			szLibName[512];
	char				szSrcDir[512];
	char				szDestDir[512];
	char				szTableDesc[512];
	CString				cSqlFile;
	CString 			cInputFile;
	CString				cOutputFile;
	int				iResult;
	CLogger				cLog;
	CString				cMessage;
	BOOL				bCreateSQL;

	cLog.Init("Log.txt");
	if (argc < 6)
	{
		cMessage.Init();
		cMessage.Append("Usage: convert-EBCDIC-CSV <table-desc-file> <library-name> <src-dir> <dest-dir> <file-name-1> ... <file-name-n>\n");
		cMessage.Append("Example: convert-EBCDIC-CSV TableDescription.txt LIB1 ebc csv FILEX AFILE\n");
		cLog.Log(cMessage.Text());
		cMessage.Kill();
		cLog.Kill();
		exit(1);
	}
	
	strcpy(szTableDesc, argv[1]);
	strcpy(szLibName, argv[2]);
	strcpy(szSrcDir, argv[3]);
	strcpy(szDestDir, argv[4]);
	
	FormatNicely(szSrcDir);
	FormatNicely(szDestDir);

	cLog.Log("Beginning conversion\n");
	
	cTableDescContainer.Init();
	if (!cTableDescContainer.AddLibrary(szTableDesc, szLibName))
	{
		cMessage.Init("Could not find a description for ");
		cMessage.Append(szLibName);
		cMessage.Append(" in ");
		cMessage.Append(szTableDesc);
		cMessage.Append("\n");
		cLog.Log(cMessage.Text());
		cMessage.Kill();
		cLog.Kill();
		exit(1);		
	}
	
	cFileConverter.Init(&cLog);

	iResult = 0;
	
	//Create SQL Command file...
	cSqlFile.Init(szDestDir);
	cSqlFile.Append("command.sql");
	
	cSQLCommand.Init(cSqlFile.Text(), szDestDir, &cLog);
	cSqlFile.Kill();

	for (int i = 5; i < argc; i++)
	{
		strcpy(szFileName, argv[i]);
		cLog.Log(szFileName);
		cLog.Log("  [");
		pcTableDesc = cTableDescContainer.Get(szFileName);
		if (!pcTableDesc)
		{
			cLog.Log("No description]\n");
			iResult = 1;
			continue;
		}
		
		cInputFile.Init(szSrcDir);
		cInputFile.Append(szFileName);
		cInputFile.Append(".ebc");

		cLog.Log(cInputFile.Text());
		cLog.Log(" ");
		if (!FileExists(cInputFile.Text()))
		{
			cLog.Log("does not exist]\n");
			iResult = 1;
			continue;
		}
		
		cOutputFile.Init(szDestDir);
		cOutputFile.Append(szFileName);
		cOutputFile.Append(".csv");

		cMessage.Init("-> ");
		cMessage.Append(cOutputFile);
		cMessage.Append("]  [");
		cLog.Log(cMessage.Text());
		cMessage.Kill();
		
		iResult |= cFileConverter.ConvertFile(cOutputFile.Text(), cInputFile.Text(), pcTableDesc, &bCreateSQL);
		if (bCreateSQL)
		{
			iResult |= cSQLCommand.Append(pcTableDesc);
		}
		cLog.Log("]\n");
		
		cInputFile.Kill();
		cOutputFile.Kill();
	}
	cSQLCommand.Kill();
	cFileConverter.Kill();

	cTableDescContainer.Kill();

	cLog.Log("Completed conversion\n");
	cLog.Kill();
	return iResult;
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
void FormatNicely(char* szDir)
{
	int iStrLen;
	
	iStrLen = strlen(szDir);
	if (iStrLen != 0)
	{
		if (szDir[iStrLen-1] == '/')
		{
			szDir[iStrLen-1] = 0;
		}
	}
	strcat(szDir, "/");
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
BOOL FileExists(char* szFile)
{
	FILE*	pFile;
	
	pFile = fopen(szFile, "rb");
	
	if (pFile)
	{
		fclose(pFile);
		return TRUE;
	}
	return FALSE;
}

