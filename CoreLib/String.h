#ifndef __STRING_H__
#define __STRING_H__
#include <stdlib.h>
#include <string.h>
#include "ArrayChar.h"
#include "ArrayTemplate.h"
#include "ArraySimple.h"


class CString
{
private:
	char*	PrivateGrow(int iNumberOfCharacters);

public:
	CArrayChar	mcText;

	void	Init(void);
	void	Init(char* sz);
	void	Init(CString sz);
	void	Init(int iLength);
	void	Init(char c, int iNumber);
	void	Kill(void);
	int		Length(void);
	void	Set(char* sz);
	void	Append(char* sz);
	void	Append(char* sz, int iLength);
	void	Append(CString sz);
	void	Append(char c);
	void	Append(int i);
	void	Append(float f);
	void	AppendNewLine(void);
	void	AppendSubString(CString sz, int iStartInclusive, int iEndExclusive);
	void	Minimize(void);
	char*	Text(void);
	BOOL	Empty(void);
	void	Append(char c, int iNumber);
	void	LeftAlign(CString sz, char cPadCharacter, int iWidth);
	void	RightAlign(CString sz, char cPadCharacter, int iWidth);
	void	LeftAlign(char* sz, char cPadCharacter, int iWidth);
	void	RightAlign(char* sz, char cPadCharacter, int iWidth);
	void	RemoveLastCharacter(void);
	BOOL	Equals(char* szString);
	BOOL	Equals(CString szString);
	BOOL	Contains(char* szString);
	void	Dump(void);
};


extern CString	gszEmptyString;


void InitEmptyString(void);
void KillEmptyString(void);


typedef CArrayTemplate<CString>		CArrayString;


#endif

