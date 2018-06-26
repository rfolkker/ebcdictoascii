#ifndef __CSV_FILE_H__
#define __CSV_FILE_H__
#include "FileExtended.h"


class CCSVFile
{
public:
	CFileExtended	mcFileExtended;
	int				miFileSize;
	char			mcSeparator;
	char			mcDelimiter;

	void Init(char cSeparator, char cDelimiter);
	void Kill(void);

	BOOL OpenFile(char* szName);
	void Close(void);
};


#endif

