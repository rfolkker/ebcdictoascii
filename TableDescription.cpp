#include "TableDescription.h"
#include "CoreLib/TextParser.h"


void CTableDescription::Init(CString Name, CString source)
{
	this->name.Init(Name);
	this->source.Init(source);
	this->fields.Init(10);
	miRowLength = 0;
	miFields = 0;
}


void CTableDescription::Kill(void)
{
	int							i;
	CFieldDescription*	pcField;

	for (i = 0; i < fields.NumElements(); i++)
	{
		pcField = fields.Get(i);
		pcField->Kill();
	}

	this->name.Kill();
	this->source.Kill();
	this->fields.Kill();
	miRowLength = 0;
	miFields = 0;
}


CFieldDescription* CTableDescription::Add(CString Name, 
													  CString lautusName, 
													  CString description, 
													  CString type, 
													  CString recordLength, 
													  CString wescoStartingPosition, 
													  CString wescoEndingPosition, 
													  CString wescoByteLength, 
													  CString numberOfDigits, 
													  CString numberOfDecimals, 
													  CString wescoText, 
													  CString numberOfFields, 
													  CString displayType, 
													  CString usedFlags, 
													  CString x)
{
	CFieldDescription*	pcDesc;

	pcDesc = fields.Add();
	pcDesc->Init(Name, lautusName, description, type, recordLength, wescoStartingPosition, wescoEndingPosition, wescoByteLength, numberOfDigits, numberOfDecimals, wescoText, numberOfFields, displayType, usedFlags, x);

	if (!miRowLength)
	{
		ParseInteger(&miRowLength, recordLength.Text());
	}
	
	if (!miFields)
	{
		ParseInteger(&miFields, numberOfFields.Text());
	}

	return pcDesc;
}


int	CTableDescription::GetRowLength(void)
{
	return miRowLength;
}


int	CTableDescription::GetNumberOfFields(void)
{
	return miFields;
}


CFieldDescription* CTableDescription::GetField(int iFieldNum)
{
	return fields.Get(iFieldNum);
}

