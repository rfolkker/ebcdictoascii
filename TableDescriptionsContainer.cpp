#include "TableDescriptionsContainer.h"
#include "CoreLib/CSVFileImutable.h"
#include <iostream>

void CTableDescriptionsContainer::Init(void)
{
	tableDescriptions.Init(10);
	mcLibrary.Init();
}


void CTableDescriptionsContainer::Kill(void)
{
	int							i;
	CTableDescription*	pcDesc;
	
	mcLibrary.Kill();
	for (i = 0; i < tableDescriptions.NumElements(); i++)
	{
		pcDesc = tableDescriptions.Get(i);
		pcDesc->Kill();
	}
}


CTableDescription* CTableDescriptionsContainer::Get(char* szTableName)
{
	int							i;
	CTableDescription*	pcDesc;

	for (i = 0; i < tableDescriptions.NumElements(); i++)
	{
		pcDesc = tableDescriptions.Get(i);
		if (pcDesc->name.Equals(szTableName))
		{
			return pcDesc;
		}
	}
	return NULL;
}


CTableDescription* CTableDescriptionsContainer::Add(CString Name, CString source)
{
	CTableDescription*  pcDesc;

	pcDesc = tableDescriptions.Add();
	pcDesc->Init(Name, source);
	return pcDesc;
}


BOOL CTableDescriptionsContainer::AddLibrary(char* szTableDesc, char* szLibrary)
{
	CCSVFileImmutable		csvFileReader;
	int				i;
	SCSVRowImmutable*		psCSVRow;
	int						libIndex;
	int						usedIndex;
	int 						stringxIndex;
	int 						legacyTableIndex;
	int 						legacySourceIndex;
	int 						FieldNameIndex;
	int 						descriptionIndex;
	int 						typeIndex;
	int 						lautusFieldNameIndex;
	int 						recordLengthIndex;
	int 						startingPositionIndex;
	int 						endingPositionIndex;
	int 						byteLengthIndex;
	int 						numberOfDigitsIndex;
	int 						numberOfDecimalsIndex;
	int 						textIndex;
	int 						numberOfFieldsIndex;
	int 						displayTypeIndex;
	CTableDescription*	TableDescription;
	CString					lib;
	CString					used;
	CString 					legacyTable;
	CString					stringx;
	CString 					legacySource;
	CString 					FieldName;
	CString					description;
	CString 					type;
	CString 					lautusFieldName;
	CString 					recordLength;
	CString 					startingPosition;
	CString 					endingPosition;
	CString 					byteLength;
	CString 					numberOfDigits;
	CString 					numberOfDecimals;
	CString 					text;
	CString 					numberOfFields;
	CString					displayType;

	mcLibrary.Init(szLibrary);
	csvFileReader.Init('|', 0);
	if (!csvFileReader.OpenFile(szTableDesc))
	{
		return FALSE;
	}
	csvFileReader.ReadAllLines();

	psCSVRow = csvFileReader.Get(0);
	libIndex =		psCSVRow->IndexOf("Lib");
	usedIndex =		psCSVRow->IndexOf("Used");
	stringxIndex =		psCSVRow->IndexOf("X");
	legacyTableIndex =	psCSVRow->IndexOf("File");
	legacySourceIndex =	psCSVRow->IndexOf("Source");
	FieldNameIndex =	psCSVRow->IndexOf("Field");
	descriptionIndex =	psCSVRow->IndexOf("Description");
	typeIndex =		psCSVRow->IndexOf("Field Type");
	lautusFieldNameIndex =	psCSVRow->IndexOf("LautusColumnName");
	recordLengthIndex =	psCSVRow->IndexOf("Record Len");
	startingPositionIndex = psCSVRow->IndexOf("Start Pos");
	endingPositionIndex =	psCSVRow->IndexOf("End Pos");
	byteLengthIndex =	psCSVRow->IndexOf("Bytes");
	numberOfDigitsIndex =	psCSVRow->IndexOf("Digits");
	numberOfDecimalsIndex = psCSVRow->IndexOf("Decimals");
	textIndex =		psCSVRow->IndexOf("Field Text");
	numberOfFieldsIndex =	psCSVRow->IndexOf("Number of fields");
	displayTypeIndex =	psCSVRow->IndexOf("Display Type");

	lib.Init();
	used.Init();
	legacyTable.Init();
	stringx.Init();
	legacySource.Init();
	FieldName.Init();
	description.Init();
	type.Init();
	lautusFieldName.Init();
	recordLength.Init();
	startingPosition.Init();
	endingPosition.Init();
	byteLength.Init();
	numberOfDigits.Init();
	numberOfDecimals.Init();
	text.Init();
	numberOfFields.Init();
	displayType.Init();
	for (i = 1; i < csvFileReader.NumRows(); i++)
	{
		psCSVRow = csvFileReader.Get(i);

		char* c;
		c = psCSVRow->Get(libIndex); 
		lib.Init(c);
		if ((lib.Equals(mcLibrary)) || 
			(lib.Equals("")) || 
			(lib.Equals("TESTOBJ")))
		{
			used.Init(psCSVRow->Get(usedIndex));
				stringx.Init(psCSVRow->Get(stringxIndex));
				legacyTable.Init(psCSVRow->Get(legacyTableIndex));
				legacySource.Init(psCSVRow->Get(legacySourceIndex));
				FieldName.Init(psCSVRow->Get(FieldNameIndex));
				description.Init(psCSVRow->Get(descriptionIndex));
				type.Init(psCSVRow->Get(typeIndex));
				lautusFieldName.Init(psCSVRow->Get(lautusFieldNameIndex));
				recordLength.Init(psCSVRow->Get(recordLengthIndex));
				startingPosition.Init(psCSVRow->Get(startingPositionIndex));
				endingPosition.Init(psCSVRow->Get(endingPositionIndex));
				byteLength.Init(psCSVRow->Get(byteLengthIndex));
				numberOfDigits.Init(psCSVRow->Get(numberOfDigitsIndex));
				numberOfDecimals.Init(psCSVRow->Get(numberOfDecimalsIndex));
				text.Init(psCSVRow->Get(textIndex));
				numberOfFields.Init(psCSVRow->Get(numberOfFieldsIndex));
				displayType.Init(psCSVRow->Get(displayTypeIndex));

				TableDescription = Get(legacyTable.Text());
				if (!TableDescription)
				{
					TableDescription = Add(legacyTable, legacySource);
				}

				if (stringx.Equals("2"))
				{
					TableDescription->Add(
						FieldName,
						lautusFieldName,
						description,
						type,
						recordLength,
						startingPosition,
						endingPosition,
						byteLength,
						numberOfDigits,
						numberOfDecimals,
						text,
						numberOfFields,
						displayType,
						used,
						stringx);
				}
		}
	}
	lib.Kill();
	used.Kill();
	legacyTable.Kill();
	stringx.Kill();
	legacySource.Kill();
	FieldName.Kill();
	description.Kill();
	type.Kill();
	lautusFieldName.Kill();
	recordLength.Kill();
	startingPosition.Kill();
	endingPosition.Kill();
	byteLength.Kill();
	numberOfDigits.Kill();
	numberOfDecimals.Kill();
	text.Kill();
	numberOfFields.Kill();
	displayType.Kill();

	csvFileReader.Close();
	csvFileReader.Kill();
	return TRUE;
}

