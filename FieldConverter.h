#ifndef __FIELD_CONVERTER_H__
#define __FIELD_CONVERTER_H__
#include "FieldDescription.h"
#include "CoreLib/FileExtended.h"
#include "CoreLib/Logger.h"


class CFieldConverter
{
public:
	CFieldDescription*	mpcFieldDescription;
	char*								mszOutputField;
	char*								mszInputField;
	CLogger* 						mpcLog;
	
	void 	Init(CLogger* pcLog);
	void 	Kill(void);
	int	ConvertField(char* szOutputField, char* szInputField, CFieldDescription* pcFieldDescription);
	int	ConvertNumeric(void);
	int	ConvertString(void);
	int	ConvertPackedDecimal(void);
	int	NumericError(char* szError);
	void	Dump(CString* pcString);
};

extern int Ebcdic2Ascii[]; 


#endif 

