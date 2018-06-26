#include "Logger.h"
#include <iostream>


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
void CLogger::Init(char* szFileName)
{
	mcFile.Init(EFT_DISK);
	mszFileName = szFileName;
	mcFile.Open(mszFileName, EFM_WRITE | EFM_BINARY);
	mcFile.Close();
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
void CLogger::Kill(void)
{
	mcFile.Kill();
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
void CLogger::Log(char* szMessage)
{
	Write(szMessage);
	std::cout << szMessage << std::flush;
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
void CLogger::Write(char* szMessage)
{
	mcFile.Open(mszFileName, EFM_APPEND | EFM_BINARY);
	mcFile.Write(szMessage, 1, strlen(szMessage));
	mcFile.Close();
}
