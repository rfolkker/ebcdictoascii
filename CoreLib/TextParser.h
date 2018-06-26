#ifndef __TEXT_PARSER_H__
#define __TEXT_PARSER_H__
#include <stdlib.h>
#include <stdio.h>
#include "Linux.h"
#include "ErrorTypes.h"
#include "EnumeratorTemplate.h"
//#include "Define.h"



struct STextPosition
{
	char*		mszParserPos;
	CArrayInt	mcPositions;
	
	void Init(void);
	void Kill(void);
};


class CTextParser
{
public:
	char*		mszStartOfText;
	int			miTextLen;
	char*		mszParserPos;
	char*		mszEndOfText;
	BOOL		mbCaseSensitive;
	BOOL		mbEndOfFile;
	CArrayInt	mcPositions;

	void 		Init(char* szText, int iTextLen);
	void 		Init(char* szText);
	void 		Kill(void);

	//These functions do not set errors...
	void 		StepRight(void);
	void 		StepLeft(void);
	void		TestEnd(void);
	void 		SkipWhiteSpace(void);
	void 		SkipCStyleComment(void);
	void 		SkipCPPStyleComment(void);
	void		Restart(void);
	void		SaveSettings(STextPosition* psTextPosition);
	void		LoadSettings(STextPosition* psTextPosition);
	void		PushPosition(void);
	void		PopPosition(void);
	void		PassPosition(void);

	int			Line(void);
	int			Column(void);
	
	TRISTATE 	GetExactCharacter(char c, BOOL bSkipWhiteSpace = TRUE);
	TRISTATE	GetIdentifierCharacter( char* pc, BOOL bFirst);
	TRISTATE	GetEnumeratedCharacter(char* szCharacters, char* c, BOOL bSkipWhiteSpace = TRUE);
	TRISTATE	GetCharacter( char* pc);
	TRISTATE	SkipBlock(char cOpen, char cClose);
	TRISTATE	GetEscapeCode( char* c);

	TRISTATE	GetExactIdentifier( char* szIdentifier);
	TRISTATE	GetIdentifier( char* szIdentifier);
	TRISTATE	GetString( char* szString);  //Quotes not included.
	TRISTATE	GetExactCharacterSequence( char* szSequence);

	TRISTATE	GetDigit( int* pi);
	TRISTATE	GetSign( int* pi);
	TRISTATE	GetDigits(int* pi, int* iNumDigits);
	TRISTATE	GetInteger( int* pi, int* iNumDigits = NULL);
	TRISTATE	GetFloat( float* pf);

	//Non linear functions.
	TRISTATE	FindExactIdentifier( char* szIdentifier);
	TRISTATE	FindExactCharacterSequence( char* szSequence);
	TRISTATE	FindStartOfLine(void);

	//Helper Functions.
	TRISTATE	GetHFExactIdentifierAndInteger( char* szIdentifier,  int* piInt);
	TRISTATE	GetHFExactIdentifierAndString( char* szIdentifier,  char* szString);

	template<class M>	TRISTATE	GetEnumeratorIdentifier(__CEnumeratorTemplate<M>* pcEnumerator, int* piID);
};


TRISTATE ParseInteger( int* pi, char* szText);
TRISTATE ParseFloat( float* pf, char* szText);


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
template<class M>
TRISTATE CTextParser::GetEnumeratorIdentifier(__CEnumeratorTemplate<M>* pcEnumerator, int* piID)
{
	char*					szName;
	SEnumeratorIterator		sIterator;
	int						iID;
	TRISTATE				tReturn;

	pcEnumerator->StartIteration(&sIterator, &szName, &iID, NULL);

	SkipWhiteSpace();
	while(sIterator.bValid)
	{
		tReturn = GetExactIdentifier(szName);
		if (tReturn == TRITRUE)
		{
			*piID = iID;
			return TRITRUE;
		}
		else if (tReturn == TRIERROR)
		{
			return TRIERROR;
		}
		pcEnumerator->Iterate(&sIterator, &szName, &iID, NULL);
	}
	*piID = -1;
	return TRIFALSE;
}



#endif

