#include "FieldConverter.h"
#include "CoreLib/IntegerHelper.h"
#include <iostream>


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
void CFieldConverter::Init(CLogger* pcLog)
{
	mpcLog = pcLog;
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
void CFieldConverter::Kill(void)
{
	mpcLog = NULL;
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
int CFieldConverter::ConvertField(char* szOutputField, char* szInputField, CFieldDescription* pcFieldDescription)
{
	mpcFieldDescription = pcFieldDescription;
	mszOutputField = szOutputField;
	mszInputField = szInputField;

	if (pcFieldDescription->IsPackedDecimal())
	{
		return ConvertPackedDecimal();
	}
	if (pcFieldDescription->IsNumeric())
	{
		return ConvertNumeric();
	}
	if (pcFieldDescription->IsString())
	{
		return ConvertString();
	}
	return -1;
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
int CFieldConverter::ConvertNumeric(void)
{
	BOOL	error = FALSE;
	int		in = 0;
	int		sign = 0;
	int		i;
	int		iLength;
	char	b;
	int		iNumberOfIntegers;
	int		iAppendPos;
	BOOL	bAnyNonZero;

	iLength = mpcFieldDescription->miLength;
	iNumberOfIntegers = mpcFieldDescription->NumberOfIntegers();
	iAppendPos = 0;
	bAnyNonZero = FALSE;

	for (i = 0; i < iLength; i++)
	{
		b = mszInputField[i];
		in = (int)b;

		if (in < 0)
		{
			in = 256 + in;
		}

		//Stupid hack so we don't lose the sign bit on the last byte before we mask it. :(
		sign = in & 0xf0;

		in &= 0x0f;
		if (in > 9)
		{
			return NumericError("Zoned decimal value out of range");
		}

		if ((i == iNumberOfIntegers) && (iLength != iNumberOfIntegers))
		{
			mszOutputField[iAppendPos] = '.';
			iAppendPos++;
		}

		//This is too stop many leading zeros from being added.
		if ((i >= iNumberOfIntegers-1) || (bAnyNonZero) || (in != 0))
		{
			mszOutputField[iAppendPos] = (char)in + '0';
			iAppendPos++;
			bAnyNonZero = TRUE;
		}
	}

	//Negatives are a special case.
	int test = sign >> 4;
	if (test == 0xd)
	{
		memmove(&mszOutputField[1], mszOutputField, iAppendPos);
		mszOutputField[0] = '-';
		iAppendPos++;
	}
	else if (test != 0xf)
	{
		return NumericError("Sign of zoned decimal not known");
	}
	return iAppendPos;
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
int CFieldConverter::ConvertString(void)
{
	int				iLength;
	int				i;
	unsigned char	cInput;
	unsigned char	cOutput;
	int				iAppendPos;
	BOOL			bAnyRealChars;
	int				iLastPosBeforeSpace;

	iLength = mpcFieldDescription->miLength;

	iLastPosBeforeSpace = 0;
	iAppendPos = 0;
	bAnyRealChars = FALSE;
	for (i = 0; i < iLength; i++)
	{
		cInput = mszInputField[i];
		cOutput = Ebcdic2Ascii[cInput];
		if (cOutput == ',')
		{
			cOutput = '.';
		}
		if (cOutput > 127)
		{
			cOutput = '?';
		}
		if ((cOutput == '"'))
		{
			//Do nothing.
		}
		else
		{
			if (cOutput != ' ')
			{
				bAnyRealChars = TRUE;
				iLastPosBeforeSpace = iAppendPos;
			}
			if (bAnyRealChars)
			{
				mszOutputField[iAppendPos] = cOutput;
				iAppendPos++;
			}
		}
	}
	if (bAnyRealChars)
	{
		return iLastPosBeforeSpace+1;
	}
	return 0;
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
int CFieldConverter::ConvertPackedDecimal(void)
{
	int				iIntegers;
	int				iDigits;
	int				iDecimals;
	int				i;
	BOOL				bHighNybble;
	int				iAppendPos;
	int				iDigit;
	unsigned char	cInput;
	BOOL				bAnyNonZero;
	BOOL				bRunning;
	BOOL				bNegative;
	int				iInputPos;

	iIntegers = mpcFieldDescription->NumberOfIntegers();
	iDecimals = mpcFieldDescription->miDecimals;
	iDigits = mpcFieldDescription->miDigits;
	bHighNybble = FALSE;
	iAppendPos = 0;
	bAnyNonZero = FALSE;
	bRunning = TRUE;
	bNegative = FALSE;
	iInputPos = 0;
	i = 0;

	if (iIntegers == 0)
	{
		mszOutputField[iAppendPos] = '0';
		iAppendPos++;
		mszOutputField[iAppendPos] = '.';
		iAppendPos++;
	}

	//Skip the padding nybble.
	if (iDigits % 2 == 0)
	{
		bHighNybble = TRUE;
	}

	while (bRunning)
	{
		cInput = mszInputField[iInputPos];

		bHighNybble = !bHighNybble;
		if (bHighNybble)
		{
			iDigit = GetHighNibble(cInput);
		}
		else
		{
			iDigit = GetLowNibble(cInput);
			iInputPos++;
		}

		switch (iDigit)
		{
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
			if ((iDigit != 0) || (bAnyNonZero) || (i >= iIntegers-1))
			{
				if ((i == iIntegers) && (iDecimals != 0))
				{
					mszOutputField[iAppendPos] = '.';
					iAppendPos++;
				}

				bAnyNonZero = TRUE;
				mszOutputField[iAppendPos] = (char)iDigit + '0';
				iAppendPos++;
			}
			break;

		//Positive sign
		case 0x0A:
		case 0x0C:
		case 0x0E:
		case 0x0F:
			bRunning = FALSE;
			break;

		//Negative sign
		default   :
			bRunning = FALSE;
			bNegative = TRUE;
			break;
		}
		i++;
	}

	if (bNegative)
	{
		memmove(&mszOutputField[1], mszOutputField, iAppendPos);
		mszOutputField[0] = '-';
		iAppendPos++;
	}

	return iAppendPos;
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
void CFieldConverter::Dump(CString* pcString)
{
	pcString->Append(mpcFieldDescription->type);
	pcString->Append(":");
	pcString->Append(mpcFieldDescription->Name);
	pcString->Append(" [start: ");
	pcString->Append(mpcFieldDescription->StartingPosition);
	pcString->Append(", length: ");
	pcString->Append(mpcFieldDescription->recordLength);
	pcString->Append(", digits: ");
	pcString->Append(mpcFieldDescription->numberOfDigits);
	pcString->Append(", decimals: ");
	pcString->Append(mpcFieldDescription->numberOfDecimals);
	pcString->Append("]");
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
int CFieldConverter::NumericError(char* szError)
{

	mszOutputField[0] = '-';
	mszOutputField[1] = '9';
	mszOutputField[2] = '9';
	mszOutputField[3] = '9';
	mszOutputField[4] = '9';
	mszOutputField[5] = '9';
	mszOutputField[6] = '.';
	mszOutputField[7] = '9';
	mszOutputField[8] = '9';
	return 9;
}


int Ebcdic2Ascii[] = 
{
0x00,0x01,0x02,0x03,0xFF,0x09,0xFF,0x7F, // 07
0xFF,0xFF,0xFF,0x0B,0x0C,0x0D,0x0E,0x0F, // 0F
0x10,0x11,0x12,0x13,0xFF,0xFF,0x08,0xFF, // 17
0x18,0x19,0xFF,0xFF,0xFF,0x1D,0xFF,0x1F, // 1F
0xFF,0xFF,0x1C,0xFF,0xFF,0x0A,0x17,0x1B, // 27
0xFF,0xFF,0xFF,0xFF,0xFF,0x05,0x06,0x07, // 2F
0xFF,0xFF,0x16,0xFF,0xFF,0x1E,0xFF,0x04, // 37
0xFF,0xFF,0xFF,0xFF,0x14,0x15,0xFF,0x1A, // 3F
0x20,0xFF,0x83,0x84,0x40,0xA0,0xFF,0x86, // 47
0x5C,0xA4,0xF8,0x2E,0x3C,0x28,0x2B,0x21, // 4F
0x26,0x7B,0x88,0x89,0x7D,0xA1,0x8C,0x8B, // 57
0x8D,0xE1,0x15,0x24,0x2A,0x29,0x3B,0x5E, // 5F
0x2D,0x2F,0xFF,0x8E,0xFF,0xFF,0xFF,0x8F, // 67
0x80,0xA5,0x97,0x2C,0x25,0x5F,0x3E,0x3F, // 6F
0xFF,0x90,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF, // 77
0xFF,0xE6,0x3A,0x9C,0x85,0x27,0x3D,0x22, // 7F
0xFF,0x61,0x62,0x63,0x64,0x65,0x66,0x67, // 87
0x68,0x69,0xAE,0xAF,0xFF,0xFF,0xFF,0xFF, // 8F
0x5B,0x6A,0x6B,0x6C,0x6D,0x6E,0x6F,0x70, // 97
0x71,0x72,0xA6,0xA7,0x91,0xFF,0x92,0xFF, // 9F
0xFF,0xFF,0x73,0x74,0x75,0x76,0x77,0x78, // A7
0x79,0x7A,0xAD,0xA8,0xFF,0xFF,0xFF,0xFF, // AF
0x9B,0x23,0x9D,0xF9,0xFF,0x5D,0x14,0xAC, // B7
0xAB,0xFF,0xAA,0x7C,0xFF,0x7E,0xFF,0xFF, // BF
0x82,0x41,0x42,0x43,0x44,0x45,0x46,0x47, // C7
0x48,0x49,0xFF,0x93,0x94,0x95,0xA2,0xFF, // CF
0x8A,0x4A,0x4B,0x4C,0x4D,0x4E,0x4F,0x50, // D7
0x51,0x52,0xFF,0x96,0x81,0xFF,0xA3,0x98, // DF
0x87,0xF6,0x53,0x54,0x55,0x56,0x57,0x58, // E7
0x59,0x5A,0xFF,0x99,0xFF,0xFF,0xFF,0xFF, // EF
0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37, // F7
0x38,0x39,0xFF,0x9A,0xFF,0xFF,0xFF,0xFF  // FF
};
