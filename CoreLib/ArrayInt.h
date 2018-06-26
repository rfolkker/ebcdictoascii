#ifndef __INT_ARRAY_H__
#define __INT_ARRAY_H__
#include "ArrayTemplate.h"


typedef CArrayTemplate<int> __CArrayInt;
template class CArrayTemplate<int>;
class CArrayInt : public __CArrayInt
{
public:
	int 	GetValue(int iElementPos);
	int	SafeGetValue(int iElementPos);
	int 	operator[](int iElementPos);
	void 	Add(int iElement);
	void 	QuickSort(void);
	void 	SetArrayValues(int iValue);
	int 	FindElement(int iValue);
	int 	AddIfUnique(int iValue);
	void	AddRemap(int iElementPos, int iRemapNum);
	void	Swap(int iElement1, int iElement2);
	void	RemoveRemap(int iElementPos);
};


int CompareInt(const void* arg1, const void* arg2);


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
__inline int CArrayInt::GetValue(int iElementPos)
{
	return (*(__CArrayInt::Get(iElementPos)));
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
__inline int CArrayInt::operator[](int iElementPos)
{
	return (*(__CArrayInt::Get(iElementPos)));
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
__inline int CArrayInt::SafeGetValue(int iElementPos)
{
	if ((iElementPos >= 0) && (iElementPos < miUsedElements))
	{
		return (*(__CArrayInt::Get(iElementPos)));		
	}
	return -1;
}


#endif

