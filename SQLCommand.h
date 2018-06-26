#ifndef __SQL_COMMAND_H__
#define __SQL_COMMAND_H__
#include "TableDescription.h"
#include "CoreLib/FileExtended.h"
#include "CoreLib/Logger.h"


class CSQLCommand
{
public:
	CLogger* 		mpcLog;
	CFileExtended	cFile;
	char* 			mszDestDir;
	
	void 	Init(char* szDestFile, char* szDestDir, CLogger* pcLog);
	void 	Kill(void);
	int  	Append(CTableDescription* pcTableDescription);
	char* GetColumnName(CFieldDescription* pcField);
	int	WriteCreateCommand(CTableDescription* pcTableDescription);
	int	WriteIndexCommand(CTableDescription* pcTableDescription);
	int 	WriteCopyCommand(CTableDescription* pcTableDescription);
};


#endif // __SQL_COMMAND_H__

