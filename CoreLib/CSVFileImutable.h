#ifndef __CSV_FILE_IMMUTABLE_H__
#define __CSV_FILE_IMMUTABLE_H__
#include "CSVFile.h"
#include "ArrayPointer.h"


struct SCSVRowImmutable
{
	int		iTotalSize;  //The complete size of this struct.
	int		iNumFields;  //The numver of terminating zeros in the string.
	int		iRowLength;  //The number of characters in the string including all terminating zeros.

	char*	aszFields[1];	//This is just the first fields positions, the size of the struct will be increased to contain iNumFields char*'s.
	//...
	//The size of the struct will also include all the zero terminated strings lengths.

	void 		Dump(void);
	int	 		IndexOf(char* szString);
	char*		Get(int iIndex);
};


class CCSVFileImmutable : public CCSVFile
{
public:
	CArrayPointer		mapsCSVRows;

	void 				Init(char cSeparator, char cDelimiter);
	void 				Kill(void);

	BOOL 				ReadLine(void);
	BOOL 				ReadLine(int iFileOffset, SCSVRowImmutable** ppsCSVRow);
	SCSVRowImmutable*	AllocateRow(char* szText);
	void					FreeRow(SCSVRowImmutable* psCSVRow);
	void 				ReadAllLines(void);
	void 				Dump(void);
	int					NumRows(void);
	SCSVRowImmutable*	Get(int iRowNum);
};


#endif

