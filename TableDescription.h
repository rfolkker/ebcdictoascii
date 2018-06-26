#ifndef ___TABLE_DESCRIPTION_H__
#define ___TABLE_DESCRIPTION_H__
#include "FieldDescription.h"
#include "CoreLib/ArrayTemplate.h"


typedef CArrayTemplate<CFieldDescription> CArrayFieldDescription;


class CTableDescription
{
public:
	CString 									name;
	CString 									source;
	CArrayFieldDescription		fields;
	int										miRowLength;
	int										miFields;

	void 								Init(CString Name, CString source);
	void 								Kill(void);
	CFieldDescription*	Add(CString Name, CString lautusName, CString description, CString type, CString recordLength, CString wescoStartingPosition, CString wescoEndingPosition, CString wescoByteLength, CString numberOfDigits, CString numberOfDecimals, CString wescoText, CString numberOfFields, CString displayType, CString usedFlags, CString x);
	int								GetRowLength(void);
	int								GetNumberOfFields(void);
	CFieldDescription*	GetField(int iFieldNum);
};


#endif 

