#include "TextParser.h"
#include <math.h>


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
void CTextParser::Init(char* szText)
{
	Init(szText, (int)strlen(szText));
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
void CTextParser::Init(char* szText, int iTextLen)
{
	mszStartOfText = szText;
	mszParserPos = szText;
	mszEndOfText = &mszStartOfText[iTextLen - 1];
	miTextLen = iTextLen;
	mbCaseSensitive = FALSE;
	TestEnd();
	mcPositions.Init();
	PushPosition();
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
void CTextParser::Kill(void)
{
	mszParserPos = NULL;
	mszStartOfText = NULL;
	miTextLen = 0;
	mszEndOfText = NULL;
	mcPositions.Kill();
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
void STextPosition::Init(void)
{
	mszParserPos = NULL;
	mcPositions.Init();
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
void STextPosition::Kill(void)
{
	mszParserPos = NULL;
	mcPositions.Kill();
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
void CTextParser::StepRight(void)
{
	//Can only move right if we are not sitting in the last character.
	if (mszParserPos <= mszEndOfText)
	{
		mbEndOfFile = FALSE;
		mszParserPos = &mszParserPos[1];
		return;
	}
	mbEndOfFile = TRUE;
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
void CTextParser::StepLeft(void)
{
	//Can only move right if we are not sitting in the last character.
	if (mszParserPos >= mszStartOfText)
	{
		mbEndOfFile = FALSE;
		mszParserPos = &mszParserPos[-1];
		return;
	}
	mbEndOfFile = TRUE;
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
void CTextParser::TestEnd(void)
{
	if ((mszParserPos >= mszStartOfText) && (mszParserPos <= mszEndOfText))
	{
		mbEndOfFile = FALSE;
		return;
	}
	mbEndOfFile = TRUE;
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
void CTextParser::SkipWhiteSpace(void)
{
	char	cCurrent;		

	for (;;)
	{
		if (mbEndOfFile)
		{
			return;
		}

		cCurrent = mszParserPos[0];

		//Nice clean white space...
		if ((cCurrent == ' ') || (cCurrent == '\r') || (cCurrent == '\n') || (cCurrent == '\t'))
		{
			StepRight();
		}

		//Possibly nasty comments...
		else if (cCurrent == '/')
		{
			StepRight();

			if (!mbEndOfFile)
			{
				cCurrent = mszParserPos[0];
				if (cCurrent == '*')
				{
					//Put the parser back where it was.
					StepLeft();
					SkipCStyleComment();
				}
				else if (cCurrent == '/')
				{
					//Put the parser back where it was.
					StepLeft();
					SkipCPPStyleComment();
				}
				else
				{
					//Was something other than white-space starting with /
					break;
				}
			}
		}
		else
		{
			//Was not white-space at all.
			break;
		}
	}
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
void CTextParser::SkipCStyleComment(void)
{
	char	cCurrent;		
	int		iDepth;

	iDepth = 0;

	PushPosition();
	for (;;)
	{
		if (mbEndOfFile)
		{
			PassPosition();
			return;
		}

		cCurrent = mszParserPos[0];
		if (cCurrent == '/')
		{
			StepRight();
			if (!mbEndOfFile)
			{
				cCurrent = mszParserPos[0];
				if (cCurrent == '*')
				{
					iDepth++;
				}
				else
				{
					//Wasn't a comment start... step back.
					StepLeft();
				}
			}
			else
			{
				PassPosition();
				return;
			}
		}
		else if (cCurrent == '*')
		{
			StepRight();
			if (!mbEndOfFile)
			{
				cCurrent = mszParserPos[0];
				if (cCurrent == '/')
				{
					iDepth--;
				}
				else
				{
					//Wasn't the end of a comment... step back...
					StepLeft();
				}
			}
			else
			{
				PassPosition();
				return;
			}
		}

		if (iDepth == 0)
		{
			//No more nested comments...  bail..
			return;
		}
		StepRight();
	}
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
void CTextParser::SkipCPPStyleComment(void)
{
	char	cCurrent;		

	if (mbEndOfFile)
	{
		return;
	}

	PushPosition();
	cCurrent = mszParserPos[0];

	if (cCurrent == '/')
	{
		StepRight();
		if (!mbEndOfFile)
		{
			cCurrent = mszParserPos[0];
			if (cCurrent == '/')
			{
				for (;;)
				{
					StepRight();
					if (!mbEndOfFile)
					{
						cCurrent = mszParserPos[0];

						if (cCurrent == '\r')
						{
							StepRight();
							if (!mbEndOfFile)
							{
								cCurrent = mszParserPos[0];
								if (cCurrent == '\n')
								{
									//This is the end of the line and the end of the comment.
									StepRight();
									PassPosition();
									return;
								}
								else
								{
									PassPosition();
									return;
								}
							}
							else
							{
								PassPosition();
								return;
							}
						}
						else if (cCurrent == '\n')
						{
							//This is the end of the line and the end of the comment.
							StepRight();
							PassPosition();
							return;
						}
					}
					else
					{
						PassPosition();
						return;
					}
				}
			}
			else
			{
				PopPosition();
				return;
			}
		}
		else
		{
			//Wasn't a comment.
			StepLeft();
			return;
		}
	}
	PopPosition();
	return;
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
TRISTATE CTextParser::GetExactCharacter(char c, BOOL bSkipWhiteSpace)
{
	if (bSkipWhiteSpace)
	{
		SkipWhiteSpace();
	}
	if (!mbEndOfFile)
	{
		if (mszParserPos[0] == c)
		{
			StepRight();
			return TRITRUE;
		}
		return TRIFALSE;
	}
	else
	{
		return TRIERROR;
	}
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
TRISTATE CTextParser::GetEnumeratedCharacter(char* szCharacters, char* c, BOOL bSkipWhiteSpace)
{
	int			iNumCharacters;
	char		cCurrent;
	TRISTATE	tResult;

	if (bSkipWhiteSpace)
	{
		SkipWhiteSpace();
	}

	iNumCharacters = (int)(strlen(szCharacters));
	for (int i = 0; i < iNumCharacters; i++)
	{
		cCurrent = szCharacters[i];
		tResult = GetExactCharacter(cCurrent, FALSE);
		ReturnOnError(tResult);
		if (tResult == TRITRUE)
		{
			if (c != NULL)
			{
				*c = cCurrent;
				return TRITRUE;
			}
		}
	}
	return TRIFALSE;
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
TRISTATE CTextParser::GetCharacter(char* pc)
{
	if (!mbEndOfFile)
	{
		*pc = mszParserPos[0];
		StepRight();
		return TRITRUE;
	}
	else
	{
		return TRIERROR;
	}
}



//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
TRISTATE CTextParser::GetIdentifierCharacter(char* pc, BOOL bFirst)
{
	char	cCurrent;

	if (!mbEndOfFile)
	{
		cCurrent = mszParserPos[0];
		*pc = cCurrent;
		//The first character of an identifier must be one of these...
		if (((cCurrent >= 'a') && (cCurrent <= 'z')) || ((cCurrent >= 'A') && (cCurrent <= 'Z')) || (cCurrent == '_'))
		{
			StepRight();
			return TRITRUE;
		}

		//Additional characters can also be...
		if (!bFirst)
		{
			if ((cCurrent >= '0') && (cCurrent <= '9'))
			{
				StepRight();
				return TRITRUE;
			}
		}
		return TRIFALSE;
	}
	else
	{
		return TRIERROR;
	}
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
TRISTATE CTextParser::SkipBlock(char cOpen, char cClose)
{
	//Dont care about this yet.
	return TRIERROR;
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
TRISTATE CTextParser::GetExactCharacterSequence(char* szSequence)
{
	char		cCurrent;
	int			iPos;

	iPos = 0;
	PushPosition();
	SkipWhiteSpace();

	//Make sure we're not off the end of the file.
	if (mbEndOfFile)
	{
		PopPosition();
		return TRIERROR;
	}

	for (;;)
	{
		if (!mbEndOfFile)
		{
			cCurrent = mszParserPos[0];
			if (szSequence[iPos] == 0)
			{
				//Got all the way to the NULL character.
				PassPosition();
				return TRITRUE;
			}
			if (cCurrent == szSequence[iPos])
			{
				StepRight();
				iPos++;
			}
			else
			{
				//Put the parser back where it was.
				PopPosition();
				return TRIFALSE;
			}
		}
		else
		{
			//Put the parser back where it was.
			PopPosition();
			return TRIFALSE;
		}
	}
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
TRISTATE CTextParser::GetExactIdentifier(char* szIdentifier)
{
	char		cCurrent;
	int			iPos;
	TRISTATE	tResult;

	iPos = 0;
	PushPosition();
	SkipWhiteSpace();

	//Make sure we're not off the end of the file.
	if (mbEndOfFile)
	{
		PopPosition();
		return TRIERROR;
	}

	for (;;)
	{
		if (!mbEndOfFile)
		{
			cCurrent = mszParserPos[0];
			if (szIdentifier[iPos] == 0)
			{
				//Got all the way to the NULL character.
				//If there are additional identifier characters then we do not have the right identifier.
				tResult = GetIdentifierCharacter(&cCurrent, iPos == 0);
				if (tResult == TRITRUE)
				{
					//Put the parser back where it was.
					PopPosition();
					return TRIFALSE;
				}
				PassPosition();
				return TRITRUE;
			}
			if (cCurrent == szIdentifier[iPos])
			{
				StepRight();
				iPos++;
			}
			else
			{
				//Put the parser back where it was.
				PopPosition();
				return TRIFALSE;
			}
		}
		else
		{
			//Put the parser back where it was.
			PopPosition();
			return TRIFALSE;
		}
	}
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
TRISTATE CTextParser::GetIdentifier(char* szIdentifier)
{
	char	c;
	BOOL	bFirst;
	int		iPos;

	bFirst = TRUE;
	iPos = 0;
	PushPosition();
	SkipWhiteSpace();

	//Make sure we're not off the end of the file.
	if (mbEndOfFile)
	{
		PopPosition();
		return TRIERROR;
	}

	for (;;)
	{
		if (!mbEndOfFile)
		{
			if (GetIdentifierCharacter(&c, bFirst) != TRITRUE)
			{
				if (bFirst)
				{
					szIdentifier[iPos] = 0;
					PopPosition();
					return TRIFALSE;
				}
				else
				{
					szIdentifier[iPos] = 0;
					PassPosition();
					return TRITRUE;
				}
			}
			else
			{
				szIdentifier[iPos] = c;
			}
		}
		else
		{
			if (bFirst)
			{
				PopPosition();
				return TRIERROR;
			}
			else
			{
				szIdentifier[iPos] = 0;
				PassPosition();
				return TRITRUE;
			}
		}
		bFirst = FALSE;
		iPos++;
	}
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
TRISTATE CTextParser::GetString(char* szString)
{
	int			iPos;
	char		cCurrent;
	TRISTATE	tReturn;

	PushPosition();
	SkipWhiteSpace();

	if (!mbEndOfFile)
	{
		if (GetExactCharacter('\"', FALSE))
		{
			iPos = 0;
			for (;;)
			{
				if (!mbEndOfFile)
				{
					cCurrent = mszParserPos[0];
					if (cCurrent == '\"')
					{
						szString[iPos] = 0;
						StepRight();
						PassPosition();
						return TRITRUE;
					}
					//We have an escape character...
					else if (cCurrent == '\\')
					{
						tReturn = GetEscapeCode(&szString[iPos]);
						iPos++;
						if ((tReturn == TRIFALSE) || (tReturn == TRIERROR))
						{
							PopPosition();
							return TRIERROR;
						}
					}
					else if (cCurrent == '\n')
					{
						//Just ignore new lines.
						StepRight();
					}
					else if (cCurrent == '\r')
					{
						//Just ignore carriage returns.
						StepRight();
					}
					else
					{
						szString[iPos] = cCurrent;
						iPos++;
						StepRight();
					}
				}
			}
		}
		else
		{
			//No quote so not a string.
			PopPosition();
			return TRIFALSE;
		}
	}
	else
	{
		PopPosition();
		return TRIERROR;
	}
}


TRISTATE CTextParser::GetEscapeCode(char* c)
{
	char cCurrent;

	if (!mbEndOfFile)
	{
		if (mszParserPos[0] == '\\')
		{
			StepRight();
			if (!mbEndOfFile)
			{
				cCurrent = mszParserPos[0];
				if (cCurrent == 'n')
				{
					*c = '\n';
				}
				else if (cCurrent == '\\')
				{
					*c = '\\';
				}
				else if (cCurrent == '\"')
				{
					*c = '\"';
				}
				else 
				{
					return TRIERROR;
				}
				StepRight();
				return TRITRUE;
			}
			else
			{
				return TRIERROR;
			}
		}
		else
		{
			return TRIERROR;
		}
	}
	else
	{
		return TRIERROR;
	}
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
TRISTATE CTextParser::GetDigit(int* pi)
{
	char	cCurrent;

	if (!mbEndOfFile)
	{
		cCurrent = mszParserPos[0];
		if ((cCurrent >= '0') && (cCurrent <= '9'))
		{
			*pi = (int)(cCurrent - '0');
			StepRight();
			return TRITRUE;
		}
		else
		{
			return TRIFALSE;
		}
	}
	else
	{
		return TRIERROR;
	}
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
TRISTATE CTextParser::GetSign(int* pi)
{
	char	cCurrent;

	if (!mbEndOfFile)
	{
		*pi = 1;
		cCurrent = mszParserPos[0];
		if (cCurrent == '-')
		{
			*pi = -1;
			StepRight();
			return TRITRUE;
		}
		else if (cCurrent == '+')
		{
			StepRight();
			return TRITRUE;
		}
		else return TRIFALSE;
	}
	else
	{
		return TRIERROR;
	}
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
TRISTATE CTextParser::GetInteger(int* pi, int* iNumDigits)
{
	TRISTATE	tResult;

	PushPosition();
	SkipWhiteSpace();

	//Make sure we're not off the end of the file.
	if (mbEndOfFile)
	{
		PopPosition();
		return TRIERROR;
	}

	tResult  = GetDigits(pi, iNumDigits);
	if (tResult == TRITRUE)
	{
		//Make sure there are no decimals.
		if (mszParserPos[0] == '.')
		{
			PopPosition();
			return TRIFALSE;
		}

		PassPosition();
		return TRITRUE;
	}
	PopPosition();
	return tResult;
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
TRISTATE CTextParser::GetDigits(int* pi, int* iNumDigits)
{
	int			iNum;
	int			iSign;
	int			iTemp;
	TRISTATE	tReturn;
	BOOL		bFirstDigit;
	int			i;

	PushPosition();
	SkipWhiteSpace();

	*pi = 0;
	i = 0;
	if (!mbEndOfFile)
	{
		iNum = 0;

		GetSign(&iSign);
		bFirstDigit = TRUE;
		for (;;)
		{
			if (!mbEndOfFile)
			{
				tReturn = GetDigit(&iTemp);
				if (tReturn == TRITRUE)
				{
					i++;
					iNum *= 10;
					iNum += iTemp;
				}
				else if ((tReturn == TRIFALSE) || (tReturn == TRIERROR))
				{
					if (bFirstDigit)
					{
						//might already have got a sign...  so reset the parser.
						PopPosition();
						return TRIFALSE;
					}
					iNum *= iSign;
					*pi = iNum;
					if (iNumDigits)
					{
						*iNumDigits = i;
					}
					PassPosition();
					return TRITRUE;
				}
				bFirstDigit = FALSE;
			}
			else
			{
				//Got only a sign then end of file.
				PopPosition();
				return TRIERROR;
			}
		}
	}
	else
	{
		PopPosition();
		return TRIERROR;
	}
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
TRISTATE CTextParser::GetFloat(float* pf)
{
	int			iLeft;
	int			iRight;
	TRISTATE	tReturn;
	int			iNumDecimals;
	float		fLeft;
	float		fRight;
	float		fTemp;
	BOOL		bLeft;

	PushPosition();
	SkipWhiteSpace();

	*pf = 0.0f;
	if (!mbEndOfFile)
	{
		//Try and get the mantissa.
		tReturn = GetDigits(&iLeft, NULL);
		bLeft = TRUE;

		//Just return on errors an non-numbers.
		if (tReturn == TRIFALSE)
		{
			//There may still be a decimal point...
			iLeft = 0;
			bLeft = FALSE;
		}
		else if (tReturn == TRIERROR)
		{
			PopPosition();
			return TRIERROR;
		}

		fLeft = (float)iLeft;
		tReturn = GetExactCharacter('.', FALSE);
		if (tReturn == TRITRUE)
		{
			tReturn = GetDigits(&iRight, &iNumDecimals);
			if (tReturn == TRITRUE)
			{
				fRight = (float)iRight;
				fTemp = powf(10.0f, (float)(-iNumDecimals));
				fRight *= fTemp;

				*pf = fLeft + fRight;
				PassPosition();
				return TRITRUE;
			}
			else
			{
				//A decimal point must be followed by a number.
				PopPosition();
				return TRIERROR;
			}
		}
		else
		{
			//No decimal point...
			if (!bLeft)
			{
				//No digits and no point...
				PopPosition();
				return TRIFALSE;
			}
			else
			{
				*pf = fLeft;
				PassPosition();
				return TRITRUE;
			}
		}
	}
	else
	{
		PopPosition();
		return TRIERROR;
	}
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
void CTextParser::SaveSettings(STextPosition* psTextPosition)
{
	psTextPosition->Init();
	psTextPosition->mszParserPos = mszParserPos;
	psTextPosition->mcPositions.Copy(&mcPositions);
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
void CTextParser::LoadSettings(STextPosition* psTextPosition)
{
	mszParserPos = psTextPosition->mszParserPos;
	mcPositions.Copy(&psTextPosition->mcPositions);
	TestEnd();
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
TRISTATE CTextParser::FindExactIdentifier(char* szIdentifier)
{
	char*			szPosition;
	TRISTATE		result;

	PushPosition();
	SkipWhiteSpace();

	for (;;)
	{
		szPosition = mszParserPos;
		result = GetExactIdentifier(szIdentifier);
		if (result == TRIERROR)
		{
			//We've reached the end of the file without finding the identifier.
			PopPosition();
			return TRIFALSE;
		}
		else if (result == TRIFALSE)
		{
			//Try the next actual character along.
			StepRight();
			SkipWhiteSpace();
		}
		else if (result == TRITRUE)
		{
			mszParserPos = szPosition;
			PassPosition();
			return TRITRUE;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
TRISTATE CTextParser::FindExactCharacterSequence(char* szSequence)
{
	char*			szPosition;
	TRISTATE		result;

	PushPosition();
	SkipWhiteSpace();

	for (;;)
	{
		szPosition = mszParserPos;
		result = GetExactCharacterSequence(szSequence);
		if (result == TRIERROR)
		{
			//We've reached the end of the file without finding the identifier.
			PopPosition();
			return TRIFALSE;
		}
		else if (result == TRIFALSE)
		{
			//Try the next actual character along.
			StepRight();
			SkipWhiteSpace();
		}
		else if (result == TRITRUE)
		{
			mszParserPos = szPosition;
			PassPosition();
			return TRITRUE;
		}
	}
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
void CTextParser::Restart(void)
{
	mszParserPos = mszStartOfText;
	mcPositions.Kill();
	mcPositions.Init();
	PushPosition();
	TestEnd();
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
TRISTATE CTextParser::FindStartOfLine(void)
{
	char cCurrent;

	PushPosition();

	//If we're off the end of the file we can't return the beginning of the line.
	if (mbEndOfFile)
	{
		PopPosition();
		return TRIERROR;
	}

	for (;;)
	{
		StepLeft();

		//If we have no more text then the start of the line is the start of the text.
		if (mbEndOfFile)
		{
			mszParserPos = mszStartOfText;
			PassPosition();
			return TRITRUE;
		}

		//If we get find an end of line character we've gone to far, go right again.
		cCurrent = mszParserPos[0];
		if ((cCurrent == '\n') || (cCurrent == '\r'))
		{
			StepRight();
			PassPosition();
			return TRITRUE;
		}
	}
}


// ----------------------- Helper Functions ------------------------------
//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
TRISTATE CTextParser::GetHFExactIdentifierAndInteger(char* szIdentifier, int* piInt)
{
	TRISTATE	tReturn;

	PushPosition();

	tReturn = GetExactIdentifier(szIdentifier);
	if ((tReturn == TRIERROR) || (tReturn == TRIFALSE))
	{
		PopPosition();
		return tReturn;
	}
	tReturn = GetInteger(piInt);
	if ((tReturn == TRIERROR) || (tReturn == TRIFALSE))
	{
		PopPosition();
		return tReturn;
	}

	PassPosition();
	return TRITRUE;
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
TRISTATE CTextParser::GetHFExactIdentifierAndString(char* szIdentifier, char* szString)
{
	TRISTATE	tReturn;

	PushPosition();

	tReturn = GetExactIdentifier(szIdentifier);
	if ((tReturn == TRIERROR) || (tReturn == TRIFALSE))
	{
		PopPosition();
		return tReturn;
	}
	tReturn = GetString(szString);
	if ((tReturn == TRIERROR) || (tReturn == TRIFALSE))
	{
		PopPosition();
		return tReturn;
	}

	PassPosition();
	return TRITRUE;
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
void CTextParser::PushPosition(void)
{
	mcPositions.Push((int*)&mszParserPos);
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
void CTextParser::PopPosition(void)
{
	mszParserPos = (char*)((size_t)(*(int*)mcPositions.Tail()));
	mcPositions.Pop();
	TestEnd();
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
void CTextParser::PassPosition(void)
{
	mcPositions.Pop();
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
int	CTextParser::Line(void)
{
	return 0;
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
int	CTextParser::Column(void)
{
	return 0;
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
TRISTATE ParseInteger( int* pi, char* szText)
{
	CTextParser	cTextParser;
	TRISTATE	tResult;

	cTextParser.Init(szText);
	tResult = cTextParser.GetInteger(pi);
	cTextParser.Kill();

	return tResult;
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
TRISTATE ParseFloat( float* pf, char* szText)
{
	CTextParser	cTextParser;
	TRISTATE	tResult;

	cTextParser.Init(szText);
	tResult = cTextParser.GetFloat(pf);
	cTextParser.Kill();

	return tResult;
}

