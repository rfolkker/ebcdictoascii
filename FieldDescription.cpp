#include "FieldDescription.h"
#include "CoreLib/TextParser.h"


void CFieldDescription::Init(CString Name, CString lautusName, CString description, CString type, CString recordLength, CString StartingPosition, CString EndingPosition, CString ByteLength, CString numberOfDigits, CString numberOfDecimals, CString Text, CString numberOfFields, CString displayType, CString usedFlags, CString x)
{
	this->Name.Init(Name);
	this->lautusName.Init(lautusName);
	this->description.Init(description);
	this->type.Init(type);
	this->recordLength.Init(recordLength);
	this->StartingPosition.Init(StartingPosition);
	this->EndingPosition.Init(EndingPosition);
	this->ByteLength.Init(ByteLength);
	this->numberOfDigits.Init(numberOfDigits);
	this->numberOfDecimals.Init(numberOfDecimals);
	this->Text.Init(Text);
	this->numberOfFields.Init(numberOfFields);
	this->displayType.Init(displayType);
	this->usedFlags.Init(usedFlags);
	this->x.Init(x);

	ParseInteger(&miStartPos, StartingPosition.Text());
	ParseInteger(&miLength, ByteLength.Text());
	ParseInteger(&miDigits, numberOfDigits.Text());
	ParseInteger(&miDecimals, numberOfDecimals.Text());
	ParseType(&meType, type.Text());
	miStartPos--;
}


void CFieldDescription::Kill(void)
{
	this->Name.Kill();
	this->lautusName.Kill();
	this->description.Kill();
	this->type.Kill();
	this->recordLength.Kill();
	this->StartingPosition.Kill();
	this->EndingPosition.Kill();
	this->ByteLength.Kill();
	this->numberOfDigits.Kill();
	this->numberOfDecimals.Kill();
	this->Text.Kill();
	this->numberOfFields.Kill();
	this->displayType.Kill();
	this->usedFlags.Kill();
	this->x.Kill();
}


BOOL CFieldDescription::ParseType(eEBC_Type* peType, char* szText)
{
	if (szText[0] == 'A')
	{
		*peType = ET_A;
		return TRUE;
	}
	if (szText[0] == 'S')
	{
		*peType = ET_S;
		return TRUE;
	}
	if (szText[0] == 'P')
	{
		*peType = ET_P;
		return TRUE;
	}
	return FALSE;
}


int CFieldDescription::NumberOfIntegers(void)
{
	return miDigits - miDecimals;
}


BOOL CFieldDescription::IsPackedDecimal(void)
{
	return meType == ET_P;
}


BOOL CFieldDescription::IsString(void)
{
	return meType == ET_A;
}


BOOL CFieldDescription::IsNumeric(void)
{
	return meType == ET_S;
}

