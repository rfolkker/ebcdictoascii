#ifndef __FILE_EXTENDED_H__
#define __FILE_EXTENDED_H__
#include "File.h"
#include "ArrayTemplate.h"
#include "ArrayInt.h"
#include "ArrayUnknown.h"
#include "ArraySimple.h"
#include "ErrorTypes.h"
#include "String.h"


#define CheckReadType(eType, eCheck)	eType = PrivateReadDataType(); if (eType != eCheck) { if (eType == DT_Error) { return TRIERROR; } return TRIFALSE; }
#define CheckWriteType(eType)			if (!PrivateWriteDataType(eType)) { return FALSE; }
#define CheckReadData(pvData, iSize) 	if (!Read(pvData, iSize, 1)) { return TRIERROR; }
#define CheckWriteData(pvData, iSize)	if (!Write(pvData, iSize, 1)) { return FALSE; }


enum EDataTypes
{
	DT_Error,
	DT_Data,
	DT_Chars,
	DT_String,
	DT_ArrayTemplate,
	DT_ArrayInt,
	DT_ArrayUnknown,
	DT_ArraySimple,
	DT_LinkListTemplate,
	DT_LinkListUnknown,
	DT_TreeTemplate,
	DT_TreeUnknown,
	DT_EnumeratorTemplate,
	DT_EnumeratorUnknown,

	DT_FORCE_DWORD	= 0xffffffff
};


class CFileExtended : public CFile
{
protected:
	EDataTypes			PrivateReadDataType(void);
	BOOL					PrivateWriteDataType(EDataTypes eType);
	int					PrivateReadLine(char* snz);

public:
	//General Data.
				TRISTATE	ReadData(void* pvData, int iDataSize);
				BOOL		WriteData(void* pvData, int iDataSize);

	//Strings.
				TRISTATE	ReadString(char* szString);
				BOOL		WriteString(char* szString);
				TRISTATE	ReadString(CString* szString);
				BOOL		WriteString(CString* szString);
				TRISTATE	ReadLine(CString* szString);
				TRISTATE	ReadLine(char* szString, int iMaxLength);

	//Arrays.
	template<class M>	TRISTATE	ReadArrayTemplate(CArrayTemplate<M>* pcArray);
	template<class M>	BOOL		WriteArrayTemplate(CArrayTemplate<M>* pcArray);
							TRISTATE	ReadArrayInt(CArrayInt* pcArray);
							BOOL		WriteArrayInt(CArrayInt* pcArray);
							TRISTATE	ReadArrayUnknown(CArrayUnknown* pcArray);
							BOOL		WriteArrayUnknown(CArrayUnknown* pcArray);
	template<class M>	TRISTATE	ReadArraySimple(CArraySimple<M>* pcArray);
	template<class M>	BOOL		WriteArraySimple(CArraySimple<M>* pcArray);
};


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
BOOL CFileExtended::WriteArrayTemplate(CArrayTemplate<M>* pcArray)
{
	int iElementSize;

	iElementSize = sizeof(M);
	CheckWriteType(DT_ArrayTemplate);
	CheckWriteData(&iElementSize, sizeof(int));
	CheckWriteData(pcArray, sizeof(CArrayTemplate<M>));

	if (pcArray->miUsedElements != 0)
	{
		CheckWriteData(pcArray->pvArray, pcArray->ByteSize());
	}
	return TRUE;
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
TRISTATE CFileExtended::ReadArrayTemplate(CArrayTemplate<M>* pcArray)
{
	EDataTypes	eType;
	int			iElementSize;

	CheckReadType(eType, DT_ArrayTemplate);
	CheckReadData(&iElementSize, sizeof(int));
	if (iElementSize != sizeof(M))
	{
		return TRIFALSE;
	}
	CheckReadData(pcArray, sizeof(CArrayTemplate<M>));

	if (pcArray->miUsedElements != 0)
	{
		pcArray->InitFromHeader();
		CheckReadData(pcArray->pvArray, pcArray->ByteSize());
	}
	return TRITRUE;
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
BOOL CFileExtended::WriteArraySimple(CArraySimple<M>* pcArray)
{
	int iElementSize;

	iElementSize = sizeof(M);
	CheckWriteType(DT_ArraySimple);
	CheckWriteData(pcArray, sizeof(CArraySimple<M>));

	if (pcArray->miUsedElements != 0)
	{
		CheckWriteData(pcArray->pvArray, pcArray->ByteSize());
	}
	return TRUE;
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
TRISTATE CFileExtended::ReadArraySimple(CArraySimple<M>* pcArray)
{
	EDataTypes	eType;

	CheckReadType(eType, DT_ArraySimple);
	CheckReadData(pcArray, sizeof(CArraySimple<M>));

	if (pcArray->miUsedElements != 0)
	{
		pcArray->InitFromHeader();
		CheckReadData(pcArray->pvArray, pcArray->ByteSize());
	}
	return TRITRUE;
}


#endif 

