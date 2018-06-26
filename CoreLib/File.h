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
//* File Name   : File.h                                             *
//*                                                                  *
//* Author      : Justin Paver                                       *
//*                                                                  *
//* Date        : started    : 01 April 2003                         *
//*                                                                  *
//* Description : Implements the CFile. a file class.                *
//*                                                                  *
//********************************************************************
#ifndef __CFILE_H__
#define __CFILE_H__
#include <stdio.h>
#include <stdlib.h>
#include "Linux.h"

//---------------------------------
// EFM = Enum File Mode
//---------------------------------
enum EFileModeFlags
{
	EFM_WRITE 		= 0x001,
	EFM_READ  		= 0x002,
	EFM_BINARY    	= 0x004,
	EFM_APPEND    	= 0x008,
};


//---------------------------------
// EFSO = Enum File Seek Origin
//---------------------------------
enum EFileSeekOrigin
{
	EFSO_SET     = 0,
	EFSO_CURRENT = 1,
	EFSO_END     = 2
};


enum EFileType
{
	EFT_DISK,
	EFT_MEMORY,
	EFT_DUMMY
};


//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
//%
//% Class Name : CFile          
//%
//% Description: implements an interface for file access on the PC Win32 
//%              platform: 
//% 
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
class CFile
{
private:
	FILE*	mpsFileHandle;  
	char*	mszFilename;     // filename  -OR-  a pointer to a memory location if EFM_MEMORY flag is set
	int	miMode;

public:
	int	Write(void *pBuffer, int iElementSize, int iNumElements);
	int	Read(void *pBuffer, int iElementSize, int iNumElements);
	int	Write(char* szString);

	void	Init(EFileType eFileType);
	void	Kill(void);

	BOOL	Open(char *pcFilename, int iMode);
	void*	Close();

	void	Seek(int iOffset, EFileSeekOrigin eOrigin);

	BOOL	IsEndOfFile(void);
	BOOL	IsOpen(void);
	int	GetFileLength(void);
	int	GetFilePos(void);
	int	GetFileSize(void);  //Same as GetFileLength.
};


#endif

