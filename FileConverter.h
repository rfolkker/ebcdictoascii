#ifndef __FILE_CONVERTER_H__
#define __FILE_CONVERTER_H__
#include "TableDescription.h"
#include "CoreLib/FileExtended.h"
#include "CoreLib/Logger.h"


class CFileConverter
{
public:
	CLogger* mpcLog;
	
	void 	Init(CLogger* pcLog);
	void 	Kill(void);
	int	ConvertFile(char* szOutputFile, char* szInputFile, CTableDescription* pcTableDescription, BOOL* bCSVExists);
	int	ConvertRow(char* szOuputRow, char* szInputRow, CTableDescription* pcTableDescription);
	int 	ConvertField(char* szOutputField, char* szInputField, CFieldDescription* pcFieldDescription);
	void	PrintNumberOfLines(int iNumRows);
};


#endif

