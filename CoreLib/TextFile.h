#ifndef __TEXT_FILE_H__
#define __TEXT_FILE_H__
#include "File.h"
#include "String.h"


class CTextFile
{
public:
	CFile		mcFile;
	CString		mcText;
	CString		mszFileName;

	void	Init(void);
	void	Kill(void);
	void	KillExceptBuffer(void);
    	BOOL	Read(char* szFileName);
	BOOL	Write(char* szFileName);  //Null to overwrite read filename.
	char*	Text(void);
	int	Length(void);
};


#endif

