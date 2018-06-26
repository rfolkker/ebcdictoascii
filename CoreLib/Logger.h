#ifndef __LOGGER_H__
#define __LOGGER_H__
#include "FileExtended.h"


class CLogger
{
public:
	char* 			mszFileName;
	CFileExtended	mcFile;
	
	void Init(char* szFileName);
	void Kill(void);
	void Log(char* szMessage);	
	void Write(char* szMessage);
};


#endif // __LOGGER_H__

