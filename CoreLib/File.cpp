//********************************************************************
//*                                                                  *
//*           Unpublished Source Code of FReY Interactive            *
//*                                                                  *
//********************************************************************
//*                                                                  *
//* This source code is copyrighted. Any unauthorized use of this    *
//* source code represents a violation of the rights of the author   *
//* in terms of the international copyrights act of 1974.            *
//*                                                                  *
//********************************************************************
//*                                                                  *
//* File Name   : CFile.cpp                                          *
//*                                                                  *
//* Author      : Justin Paver                                       *
//*                                                                  *
//* Date        : started    : 01 April 2003                         *
//*                                                                  *
//* Description : Implements the PC Win32 EFile interface            *
//*                                                                  *
//********************************************************************
#include "File.h"
#include <string.h>


int	__FEOF(FILE* psFile)
{
	return (feof(psFile)!=0);
}


long __FSize(FILE* psFile)
{
	long	iCurrPos;
	long	iFileSize;

    iCurrPos = ftell(psFile);
	fseek(psFile, 0, SEEK_END);
	iFileSize = ftell(psFile);
	fseek(psFile, iCurrPos, SEEK_SET);
	return iFileSize;
}


//====================================================================================
//=
//= Function Name: CFile::CFile()
//= 
//= Parameters   : - 
//= 
//= Returns      : - 
//=
//= Description  : Constructor for the initialization of an EFile
//=
//====================================================================================
void CFile::Init(EFileType eFileType)
{
	memset(this, 0, sizeof(CFile));
}


void CFile::Kill(void)
{
	memset(this, 0, sizeof(CFile));
}


//====================================================================================
//=
//= Function Name: CFile::Open
//= 
//= Parameters   : pcFilename - the name of the file to open or
//=								a pointer to the block of memory to use.
//=                iMode      - the mode to open the file as.
//=                             flags are : EFM_READONLY, EFM_BINARY
//=           
//= Returns      : true if successful, false if not.
//=
//= Description  : Opens the file specified with a specific mode.
//=
//====================================================================================
BOOL CFile::Open(char* szFilename, int iMode)
{
	char pcMode[4];

	if (mpsFileHandle != NULL)
	{
		Close();
	}
	
	if (iMode & EFM_READ)
	{
		if (iMode & EFM_BINARY)
			strcpy(pcMode,"rb");
		else
			strcpy(pcMode,"r");
	}
	else if (iMode & EFM_WRITE)
	{
		if (iMode & EFM_BINARY)
			strcpy(pcMode,"wb");
		else
			strcpy(pcMode,"w");
	}
	else if (iMode & EFM_APPEND)
	{
		if (iMode & EFM_BINARY)
			strcpy(pcMode,"ab");
		else
			strcpy(pcMode,"a");
	}

	mszFilename = szFilename; 
	mpsFileHandle = fopen(szFilename, pcMode);
	if (mpsFileHandle)
	{
		return 1;
	}
	return 0;
}


//====================================================================================
//=
//= Function Name: CFile::Close
//= 
//= Parameters   : -
//= 
//= Returns      : true if successful, false if not.
//=
//= Description  : Closes the file (if it is already open)
//=
//====================================================================================
void* CFile::Close()
{
	void*	p;

	p = NULL;
	if (mpsFileHandle != NULL)
	{
		p = (void*)(size_t)fclose(mpsFileHandle);
		mpsFileHandle = NULL;
	}
	return p;
}


//====================================================================================
//=
//= Function Name: CFile::IsEndOfFile
//= 
//= Parameters   : -
//= 
//= Returns      : true if file is at the end of the file, false if not
//=
//= Description  : returns whether at the end of the file or not
//=
//====================================================================================
BOOL CFile::IsEndOfFile(void)
{
	return (__FEOF(mpsFileHandle) != 0);
}


//====================================================================================
//=
//= Function Name: CFile::Write
//= 
//= Parameters   : pBuffer - pointer to a buffer of memory to write to the file
//=                iElementSize - the size of each element to write (in the buffer) 
//=                iNumElements - the number of elements to write (in the buffer)
//=
//= Returns      : the number of elements actually written.
//=
//= Description  : writes a buffer from memory to a file
//=
//====================================================================================
int CFile::Write(void *pBuffer, int iElementSize, int iNumElements)
{
	return (int)fwrite(pBuffer, iElementSize, iNumElements, mpsFileHandle);
}


int CFile::Write(char* szString)
{
	return (int)fwrite(szString, 1, strlen(szString), mpsFileHandle);
}


//====================================================================================
//=
//= Function Name: CFile::Read
//= 
//= Parameters   : pBuffer - pointer to a buffer of memory to read into from the file
//=                iElementSize - the size of each element to read (in the buffer) 
//=                iNumElements - the number of elements to read (in the buffer)
//=
//= Returns      : the number of elements actually read.
//=
//= Description  : Reads from file into a memory buffer
//=
//====================================================================================
int CFile::Read(void *pBuffer, int iElementSize, int iNumElements)
{
	return ((int)fread(pBuffer, iElementSize, iNumElements, mpsFileHandle));
}


//====================================================================================
//=
//= Function Name: CFile::Seek
//= 
//= Parameters   : iOffset - the offset to seek to within the file
//=                iOrigin - specifies the origin of the seek. 
//=                          values are EFSO_SET, EFSO_END, EFSO_CURRENT
//= 
//= Returns      : -
//=
//= Description  : Seeks to an offset relative to an origin within the file.
//=
//====================================================================================
void   CFile::Seek(int iOffset, EFileSeekOrigin eOrigin)
{
	fseek(mpsFileHandle, iOffset, (eOrigin == EFSO_SET) ? SEEK_SET :((eOrigin == EFSO_END) ? SEEK_END : SEEK_CUR));
}


//====================================================================================
//=
//= Function Name: CFile::IsOpen()
//= 
//= Returns      : true if a file is currently open.
//=
//= Description  : Seeks to an offset relative to an origin within the file.
//=
//====================================================================================
BOOL  CFile::IsOpen(void)
{
	return (mpsFileHandle != NULL);
}


//====================================================================================
//=
//= Function Name: CFile::FileLength
//= 
//= Returns      : the length of the currently open file
//=
//= Description  : gets the length (in bytes) of the file
//=
//====================================================================================
int CFile::GetFileLength(void)
{
	return (__FSize(mpsFileHandle));
}


//====================================================================================
//=
//= Function Name: CFile::FileLength
//= 
//= Returns      : the length of the currently open file
//=
//= Description  : gets the length (in bytes) of the file
//=
//====================================================================================
int CFile::GetFileSize(void)
{
	return (__FSize(mpsFileHandle));
}


//====================================================================================
//=
//= Function Name: CFile::FilePos
//= 
//= Returns      : integer position in the file
//=
//= Description  : the position in the current file we are currently at
//=
//====================================================================================
int CFile::GetFilePos()
{
	return ftell(mpsFileHandle);
}



