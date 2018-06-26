#ifndef ___FIELD_DESCRIPTION_H__
#define ___FIELD_DESCRIPTION_H__
#include "CoreLib/String.h"



enum eEBC_Type
{
	ET_A,
	ET_S,
	ET_P
};


class CFieldDescription
{
public:
	CString 	Name;
	CString 	lautusName;
	CString 	description;
	CString 	type;
	CString 	recordLength;
	CString 	StartingPosition;
	CString 	EndingPosition;
	CString 	ByteLength;
	CString 	numberOfDigits;
	CString 	numberOfDecimals;
	CString 	Text;
	CString 	numberOfFields;
	CString 	displayType;
	CString 	usedFlags;
	CString 	x;
	int		miStartPos;
	int		miLength;
	int		miDigits;
	int		miDecimals;

	eEBC_Type	meType;

	void Init(CString Name, CString lautusName, CString description, CString type, CString recordLength, CString StartingPosition, CString EndingPosition, CString ByteLength, CString numberOfDigits, CString numberOfDecimals, CString Text, CString numberOfFields, CString displayType, CString usedFlags, CString x);
	void Kill(void);
	BOOL ParseType(eEBC_Type* peType, char* szText);
	int  NumberOfIntegers(void);
	BOOL IsPackedDecimal(void);
	BOOL IsString(void);
	BOOL IsNumeric(void);
};


#endif 

