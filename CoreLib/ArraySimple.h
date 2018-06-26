#ifndef __ARRAY_SIMPLE_H__
#define __ARRAY_SIMPLE_H__
#include <stdlib.h>
#include <string.h>


template <class M>
class CArraySimple
{
public:
	M*		pvArray;
	int		miUsedElements;

	void	Init(void);
	void	Init(int iIgnored);
	void	Kill(void);
	void	Allocate(int iNum);
	M*		Add(void);
	void	Add(M* pvData);
	M*		InsertAt(int iElementPos);
	void	InsertAt(M* pvData, int iElementPos);
	M&		operator[](int iElementPos);
	M*		SafeGet(int iElementPos);
	M*		Get(int iElementPos);
	void	RemoveAt(int iElementPos);
	void	RemoveTail(void);
	int		ByteSize(void);
	void	InitFromHeader(void);
	void	SetArraySize(int iNum);
	M*		SetArraySize(int iNum, int iClearValue);
	int		NumElements(void);
	void 	Copy(CArraySimple<M>* array);
//	void	QuickSort(int(__cdecl* Func)(const void*, const void*));
//	void	InsertIntoSorted(int(__cdecl* Func)(const void*, const void*), M* pvElement);
	int 	FindElement(M* pData);
	int 	FindElementWithKey(M* pData, int iKeyOffset, int iKeySize);
	int		FindElementWithIntKey(int iKey, int iKeyOffset);
//	int		FindElementInSorted(M* pData, int(__cdecl* Func)(const void*, const void*), int bFindIfNone);
//	int		BinarySearch(M* pData, int iLeft, int iRight, int(__cdecl* Func)(const void*, const void*), int bFindIfNone);
};


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
void CArraySimple<M>::Init(void)
{
	miUsedElements = 0;
	pvArray = NULL;
}

//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
void CArraySimple<M>::Init(int iIgnored)
{
	Init();
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
void CArraySimple<M>::Kill(void)
{
	free(pvArray);
}

//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
void CArraySimple<M>::Allocate(int iNum)
{
	Init();
	SetArraySize(iNum);
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
M* CArraySimple<M>::Add(void)
{
	miUsedElements++;
	pvArray = (M*)realloc(pvArray, miUsedElements * sizeof(M));
	return &pvArray[miUsedElements-1];
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
void CArraySimple<M>::Add(M* pvData)
{
	M* pvTemp;

	pvTemp = Add();
	memcpy(pvTemp, pvData, sizeof(M));
}



//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
M& CArraySimple<M>::operator[](int iElementPos)
{
	return pvArray[iElementPos];
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
M* CArraySimple<M>::SafeGet(int iElementPos)
{
	if ((iElementPos < 0) || (iElementPos >= miUsedElements))
	{
		return NULL;
	}
	else
	{
		return &pvArray[iElementPos];
	}
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
M* CArraySimple<M>::Get(int iElementPos)
{
	return &pvArray[iElementPos];
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
void CArraySimple<M>::RemoveAt(int iElementPos)
{
	M*	pvEnd;
	M*	pvElement;

	miUsedElements--;
	if (iElementPos < miUsedElements)
	{
		//Order is not preserved.
		pvEnd = Get(miUsedElements);
		pvElement = Get(iElementPos);
		memcpy(pvElement, pvEnd, sizeof(M));
	}
	pvArray = (M*)realloc(pvArray, miUsedElements * sizeof(M));
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
void CArraySimple<M>::RemoveTail(void)
{
	RemoveAt(miUsedElements - 1);
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
int CArraySimple<M>::ByteSize(void)
{
	return miUsedElements * sizeof(M);
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
void CArraySimple<M>::InitFromHeader(void)
{
	//This function assumes that the value of pvArray was invalid.
	pvArray = (M*)malloc(miUsedElements * sizeof(M));
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
void CArraySimple<M>::SetArraySize(int iNum)
{
	if (miUsedElements != iNum)
	{
		miUsedElements = iNum;
		pvArray = (M*)realloc(pvArray, miUsedElements * sizeof(M));
	}
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
M* CArraySimple<M>::SetArraySize(int iNum, int iClearValue)
{
	int		iOldUsed;
	void*	pvClearStart;
	int		iClearSize;

	if (miUsedElements != iNum)
	{
		iOldUsed = -1;
		if (iNum > miUsedElements)
		{
			iOldUsed = miUsedElements;
		}
		miUsedElements = iNum;
		pvArray = (M*)realloc(pvArray, miUsedElements * sizeof(M));

		if (iOldUsed != -1)
		{
			pvClearStart = (void*)((size_t)((int)((size_t)pvArray) + (iOldUsed * sizeof(M))));
			iClearSize = (miUsedElements - iOldUsed) * sizeof(M);
			memset(pvClearStart, iClearValue, iClearSize);
			return (M*)pvClearStart;
		}
	}

	//No new elements were added.
	return NULL;
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
int CArraySimple<M>::NumElements(void)
{
	return miUsedElements;
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
void CArraySimple<M>::Copy(CArraySimple* array)
{
	Kill();
	SetArraySize(array->miUsedElements);
	memcpy(pvArray, array->pvArray, miUsedElements * sizeof(M));
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
/*template<class M>
void CArraySimple<M>::QuickSort(int(__cdecl* Func)(const void*, const void*))
{
	qsort((void*)pvArray, miUsedElements, sizeof(M), Func);
}*/


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
/*template<class M>
void CArraySimple<M>::InsertIntoSorted(int(__cdecl* Func)(const void*, const void*), M* pvElement)
{
	int	iPos;

	iPos = FindElementInSorted(pvElement, Func, 1);
	if (iPos < miUsedElements)
	{
		InsertAt(pvElement, iPos);
	}
	else
	{
		Add(pvElement);
	}
}
*/

//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
/*
template<class M>
int CArraySimple<M>::FindElementInSorted(M* pData, int(__cdecl* Func)(const void*, const void*), int bFindIfNone)
{
	if (miUsedElements == 0)
	{
		if (bFindIfNone)
		{
			return 0;
		}
		return -1;
	}
	return BinarySearch(pData, 0, miUsedElements - 1, Func, bFindIfNone);
}
*/

//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////

/*template<class M>
int CArraySimple<M>::BinarySearch(M* pData, int iLeft, int iRight, int(__cdecl* Func)(const void*, const void*), int bFindIfNone)
{
	int		iMiddle;
	int		iResultMiddle;

	iResultMiddle = 0;
	while (iLeft <= iRight)
	{
		iMiddle = (iLeft + iRight) >> 1; //Divide by 2
		iResultMiddle = Func(pData, Get(iMiddle));
		if (iResultMiddle == 0)
		{
			return iMiddle;
		}
		else if (iResultMiddle < 0)
		{
			iRight = iMiddle - 1;
		}
		else
		{
			iLeft = iMiddle + 1;
		}
	}

	if (bFindIfNone)
	{
		if (iResultMiddle > 0)
		{
			return iMiddle + 1;
		}
		return iMiddle;
	}
	return -1;  
}
*/

//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
M* CArraySimple<M>::InsertAt(int iElementPos)
{
	M* ptr;

	//Grow the array.
	Add();

	//This assumes that iElementPos is within the array (or the last element).
	ptr = (M*)((size_t)pvArray + iElementPos * sizeof(M));
	memmove((M*)((size_t)ptr + sizeof(M)), ptr, sizeof(M) * (miUsedElements - 1 - iElementPos));
	return ptr;
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
void CArraySimple<M>::InsertAt(M* pvData, int iElementPos)
{
	M* ptr;

	ptr = InsertAt(iElementPos);
	memcpy(ptr, pvData, sizeof(M));
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
int CArraySimple<M>::FindElement(M* pData)
{
	int		i;
	M*	pPos;

	pPos = pvArray;

	for (i=0; i < miUsedElements; i++)
	{
		if (memcmp(pPos, pData, sizeof(M))==0)
		{
			return i;
		}
		pPos = (M*)((size_t)pPos + sizeof(M));
	}
	return -1;
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
int CArraySimple<M>::FindElementWithKey(M* pData, int iKeyOffset, int iKeySize)
{
	int		i;
	M*	pPos;

	pPos = pvArray;

	for (i=0; i < miUsedElements; i++)
	{
		if (memcmp((M*)((size_t)pPos + iKeyOffset), (M*)((size_t)pData + iKeyOffset), iKeySize)==0)
		{
			return i;
		}
		pPos = (M*)((size_t)pPos + sizeof(M));
	}
	return -1;
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
int	CArraySimple<M>::FindElementWithIntKey(int iKey, int iKeyOffset)
{
	int		i;
	void*	pPos;

	pPos = (void*)((size_t)((int)((size_t)pvArray) + iKeyOffset));
	for (i=0; i < miUsedElements; i++)
	{
		if (*((int*)pPos) == iKey)
		{
			return i;
		}
		pPos = (void*)((size_t)pPos + sizeof(M));
	}
	return -1;
}


#endif

