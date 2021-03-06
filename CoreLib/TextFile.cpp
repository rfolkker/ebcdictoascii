#include "TextFile.h"

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
void CTextFile::Init(void)
{
	mcFile.Init(EFT_DISK);
	mcText.Init(1024);
	mszFileName.Init();
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
void CTextFile::Kill(void)
{
	mcText.Kill();
	KillExceptBuffer();
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
void CTextFile::KillExceptBuffer(void)
{
	mcFile.Kill();
	mszFileName.Kill();
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
BOOL CTextFile::Read(char* szFileName)
{
	int		iSize;
	
    if (szFileName)
	{
		mszFileName.Init(szFileName);
		if (mcFile.Open(mszFileName.Text(), EFM_READ | EFM_BINARY))
		{
			iSize = mcFile.GetFileSize();

			mcText.mcText.GrowToNumElements(iSize+1);
			mcFile.Read(mcText.mcText.pvArray, iSize, 1);
			mcFile.Close();
			mcText.mcText.pvArray[iSize] = 0;
			return TRUE;
		}
	}
	return FALSE;
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
BOOL CTextFile::Write(char* szFileName)
{
	//Not yet written.
	return FALSE;
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
char* CTextFile::Text(void)
{
	return mcText.Text();
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
int CTextFile::Length(void)
{
	return mcText.Length();
}

