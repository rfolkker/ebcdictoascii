#ifndef __TEMPLATE_ARRAY_H__
#define __TEMPLATE_ARRAY_H__
#include <stdlib.h>
#include <string.h>
#include "Linux.h"


template<class M>
class __CArrayTemplate
{
protected:
	void 		SetAllocateSize(int iSize);
	void 		SetArraySize(int iNumElements);
	void*		Malloc(int iMemSize);
	void			Free(void* pvMem);
	void*		Realloc(void* pvMem, int iMemSize);
	void		CopyArrayInto(__CArrayTemplate<M>* array, int iElementPos);

public:
	int			miElementSize;	
	int			miNumElements;
	int			miUsedElements;
	int			miChunkSize;	 //Number of elements to allocate in a chunk.
	M*			pvArray;		

	void 		InitFromHeader(void);
	M*			Add(void);
	void			Add(M* pvData);
	M* 			AddGetPos(int* piPos);
	void		Set(int iElementPos, M* pvData);
	M* 			InsertAt(int iElementPos);
	void 		InsertAt(M* pvData, int iElementPos);
	void		PrivateRemoveAt(int iElementPos, int bPreserveOrder, int iDataSize);
	void 		RemoveAt(int iElementPos, int bPreserveOrder = 1);
	void 		RemoveTail(void);
	void 		Finalise(void);
	void 		Kill(void);
	void		Unuse(void);
	void 		Zero(void);
	void 		Copy(__CArrayTemplate<M>* array);
	int 		ByteSize(void);
	int 		ElementSize(void);
	int			NumElements(void);
	void		GrowByChunk(void);
	void		GrowToAtLeastNumElements(int iNumElements);  //ie:  Don't shrink the array.
	int			GrowToNumElements(int iNumElements);
	int			GrowByNumElements(int iNumElements);
	void		SetUsedElements(int iNumElements);
	M*			Get(int iElementPos);
	M*			SafeGet(int iElementPos);
	int			GetAdjustedPosition(int iIndex);
	int			Equals(__CArrayTemplate<M>* pcTemplateArray);  //
	void		InsertArrayAfterEnd(__CArrayTemplate<M>* pcTemplateArray);
	void		InsertArrayBeforeStart(__CArrayTemplate<M>* pcTemplateArray);
	void		InsertArrayAt(__CArrayTemplate<M>* pcTemplateArray, int iElementPos);
	M*			InsertNumElementsAt(int iNumElements, int iElementPos);
	
	//Stack functions.
	void 		PushCopy(void);	
	void 		Push(M* pvElement);  
	M*			Push(void);  
	void		LoadStack(M* pvElement);
	void		Pop(void);
	void		Pop(M* pvData);
	M*			Tail(void);

	//Enumerator style functions.
	int 		AddIfUnique(M* pData);
	int 		AddIfUniqueKey(M* pData, int iKeyOffset, int iKeySize);
	int 		FindElement(M* pData);
	int 		FindElementWithKey(M* pData, int iKeyOffset, int iKeySize);
	int			FindElementWithIntKey(int iKey, int iKeyOffset);

	//Sorting functions.
	void		Swap(int iElement1, int iElement2);
//	void		QuickSort(int(__cdecl*)(const void*, const void*));
//	void		BubbleSort(int(__cdecl*)(const void*, const void*));
//	void		InsertIntoSorted(int(__cdecl*)(const void*, const void*), M* pvElement);
//	int			FindElementInSorted(M* pData, int(__cdecl*)(const void*, const void*), int bFindIfNone = 0);

	//Get malloc size.
	int			MallocSize(void);
};


template<class M>
class CArrayTemplate : public __CArrayTemplate<M>
{
public:
	void 		Init(void);
	void 		Init(int iChunkSize);
	void 		ReInit(void);
	void 		Allocate(int iChunkSize);
	void 		Allocate(int iChunkSize, int iNumElements);
	M&			operator[](int iElementPos);
	M*			Get(int iElementPos);
};


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
void* __CArrayTemplate<M>::Malloc(int iMemSize)
{
	return malloc(iMemSize);
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
void __CArrayTemplate<M>::Free(void* pvMem)
{
	free(pvMem);
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
void* __CArrayTemplate<M>::Realloc(void* pvMem, int iMemSize)
{
	return realloc(pvMem, iMemSize);
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
void __CArrayTemplate<M>::InitFromHeader(void)
{
	//This function assumes that the value of pvArray was invalid.
	pvArray = (M*)Malloc(miElementSize * miNumElements);		
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
void __CArrayTemplate<M>::SetAllocateSize(int iSize)
{
	miChunkSize = iSize;
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
M* __CArrayTemplate<M>::SafeGet(int iElementPos)
{
	if ((iElementPos < 0) || (iElementPos >= miUsedElements))
	{
		return NULL;
	}
	else
	{
		return Get(iElementPos);
	}
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
void __CArrayTemplate<M>::SetArraySize(int iNumElements)
{
	M* ptr;

	if (iNumElements == 0)
	{
		Free(pvArray);
		pvArray = NULL;
		miNumElements = 0;
	}
	else
	{
		ptr = (M*)Realloc(pvArray, miElementSize * iNumElements);
		pvArray = ptr;
		miNumElements = iNumElements;
	}
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
M* __CArrayTemplate<M>::Add(void)
{
	miUsedElements++;
	if (miUsedElements>miNumElements)
	{
		SetArraySize(miNumElements + miChunkSize);
	}

	return Tail();
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
M* __CArrayTemplate<M>::AddGetPos(int* piPos)
{
	miUsedElements++;
	if (miUsedElements > miNumElements)
	{
		SetArraySize(miNumElements+miChunkSize);
	}
	(*piPos) = miUsedElements - 1;
	return Tail();
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
void __CArrayTemplate<M>::Add(M* pvData)
{
	M*	pvTemp;

	pvTemp = Add();
	memcpy((M*)((size_t)pvArray + (miUsedElements-1) * miElementSize), pvData, miElementSize);
}

//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
void __CArrayTemplate<M>::Set(int iElementPos, M* pvData)
{
	M*	pvTemp;

	pvTemp = Get(iElementPos);
	memcpy((M*)((size_t)pvArray + (iElementPos) * miElementSize), pvData, miElementSize);
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
M* __CArrayTemplate<M>::InsertAt(int iElementPos)
{
	M* ptr;

	//This assumes that iElementPos is within the array (or the last element).
	miUsedElements++;
	if (miUsedElements > miNumElements)
	{
		SetArraySize(miNumElements + miChunkSize);
	}
	ptr = (M*)((size_t)pvArray + iElementPos * miElementSize);

	memmove((M*)((size_t)ptr + miElementSize), ptr, miElementSize * (miUsedElements - 1 - iElementPos));
	return ptr;
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
void __CArrayTemplate<M>::InsertAt(M* pvData, int iElementPos)
{
	M* ptr;

	ptr = InsertAt(iElementPos);
	memcpy(ptr, pvData, miElementSize);
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
void __CArrayTemplate<M>::PrivateRemoveAt(int iElementPos, int bPreserveOrder, int iDataSize)
{
	M*	ptr;
	M*	pvEnd;

	miUsedElements--;
	ptr = (M*)((size_t)pvArray + iElementPos * iDataSize);

	if (iElementPos < miUsedElements)  //Caution!!! Untested.
	{
		if (bPreserveOrder)
		{
			//If the order of elements is to be preserverd then move all the elements back one.
			memmove(ptr, (M*)((size_t)ptr + iDataSize), iDataSize * (miUsedElements - iElementPos));
		}
		else
		{
			//If the order is unimportant then just move the last element to the empty.
			pvEnd = (M*)((size_t)pvArray + miUsedElements * iDataSize);
			memcpy(ptr, pvEnd, iDataSize);
		}
	}

	if (miUsedElements == miNumElements - miChunkSize)
	{
		SetArraySize(miNumElements - miChunkSize);
	}
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
void __CArrayTemplate<M>::RemoveAt(int iElementPos, int bPreserveOrder)
{
	PrivateRemoveAt(iElementPos, bPreserveOrder, miElementSize);
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
void __CArrayTemplate<M>::RemoveTail(void)
{
	RemoveAt(miUsedElements - 1);
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
void __CArrayTemplate<M>::Finalise(void)
{
	SetArraySize(miUsedElements);
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
void __CArrayTemplate<M>::Kill(void)
{
	if (pvArray != NULL)
	{
		Free(pvArray);
		miNumElements = 0;
		miUsedElements = 0;
	}
	pvArray = NULL;
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
void __CArrayTemplate<M>::Unuse(void)
{
	miUsedElements = 0;
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
void __CArrayTemplate<M>::Zero(void)
{
	if (miUsedElements != 0)
	{
		memset(pvArray, 0, miUsedElements * miElementSize);
	}
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
void __CArrayTemplate<M>::Copy(__CArrayTemplate* array)
{
	Kill();
	miNumElements = array->miNumElements;
	miUsedElements = array->miUsedElements;
	miChunkSize = array->miChunkSize;
	SetArraySize(miNumElements);
	memcpy(pvArray, array->pvArray, miUsedElements * miElementSize);
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
void __CArrayTemplate<M>::PushCopy(void)
{
	M* pv;

	pv = Add();
	if (miUsedElements == 1)
	{
		memset(pv, 0, miElementSize);
	}
	else
	{
		memcpy(pv, (M*)((size_t)pv - miElementSize), miElementSize);
	}
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
void __CArrayTemplate<M>::Push(M* pvElement)
{
	M* pv;
	pv = Add();
	memcpy(pv, pvElement, miElementSize);
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
M* __CArrayTemplate<M>::Push(void)
{
	return Add();
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
void __CArrayTemplate<M>::LoadStack(M* pvElement)
{
	M* pv;
	pv = Tail();
	memcpy(pv, pvElement, miElementSize);
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
void __CArrayTemplate<M>::Pop(M* pvElement)
{
	memcpy(pvElement, (M*)((size_t)pvArray + (miUsedElements-1) * miElementSize), miElementSize);
	RemoveTail();
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
void __CArrayTemplate<M>::Pop(void) 
{
	RemoveTail();
}	


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
M* __CArrayTemplate<M>::Tail(void) 
{
	if (miUsedElements == 0)
	{
		return NULL;
	}
	return Get(miUsedElements-1);
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
int __CArrayTemplate<M>::ByteSize()
{
	return miUsedElements * miElementSize;
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
void __CArrayTemplate<M>::GrowByChunk(void)
{
	miUsedElements += miChunkSize;
	SetArraySize(miNumElements + miChunkSize);
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
int __CArrayTemplate<M>::AddIfUnique(M* pData)
{
	int iElementNum;
	
	iElementNum = FindElement(pData);
	if (iElementNum == -1)
	{
		Add(pData);
	}
	return iElementNum;
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
int __CArrayTemplate<M>::FindElement(M* pData)
{
	int		i;
	M*	pPos;

	pPos = pvArray;

	for (i=0; i < miUsedElements; i++)
	{
		if (memcmp(pPos, pData, miElementSize)==0)
		{
			return i;
		}
		pPos = (M*)((size_t)pPos + miElementSize);
	}
	return -1;
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
int __CArrayTemplate<M>::AddIfUniqueKey(M* pData, int iKeyOffset, int iKeySize)
{
	int	iElementNum;

	iElementNum = FindElementWithKey(pData, iKeyOffset, iKeySize);
	if (iElementNum == -1)
	{
		Add(pData);
		iElementNum = miUsedElements - 1;
	}
	return iElementNum;
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
int __CArrayTemplate<M>::FindElementWithKey(M* pData, int iKeyOffset, int iKeySize)
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
		pPos = (M*)((size_t)pPos + miElementSize);
	}
	return -1;
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
int	__CArrayTemplate<M>::FindElementWithIntKey(int iKey, int iKeyOffset)
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
		pPos = (void*)((size_t)pPos + miElementSize);
	}
	return -1;
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
int __CArrayTemplate<M>::GrowToNumElements(int iNumElements)
{
	int	iOldUsedElements;

	iOldUsedElements = miUsedElements;
	SetUsedElements(iNumElements);
	return iOldUsedElements;
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
void __CArrayTemplate<M>::GrowToAtLeastNumElements(int iNumElements)
{
	if (miUsedElements < iNumElements)
	{
		GrowToNumElements(iNumElements);
	}
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
int __CArrayTemplate<M>::GrowByNumElements(int iNumElements)
{
	int	iOldUsedElements;

	iOldUsedElements = miUsedElements;
	SetUsedElements(iNumElements + miUsedElements);
	return iOldUsedElements;
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
void __CArrayTemplate<M>::SetUsedElements(int iNumElements)
{
	int	iNumAllocations;
	int iNumLeftOvers;

	miUsedElements = iNumElements;

	//Find how many allocation segments are needed.
	iNumAllocations = iNumElements / miChunkSize;
	iNumLeftOvers = iNumElements % miChunkSize;

	//If there are any additional elements then allocate a new chunk for them.
	if (iNumLeftOvers > 0)
	{
		iNumAllocations++;
	}

	//Num elements is the number of elements which memory has been allocated for, not the number of used elements.
	miNumElements = iNumAllocations * miChunkSize;
	SetArraySize(miNumElements);
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
int __CArrayTemplate<M>::Equals(__CArrayTemplate<M>* pcTemplateArray)
{
	if (pcTemplateArray->miUsedElements == miUsedElements)
	{
		if (memcmp(pcTemplateArray->pvArray, pvArray, ByteSize()) == 0)
		{
			return 1;
		}
	}
	return 0;
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
void __CArrayTemplate<M>::Swap(int iElement1, int iElement2)
{
	M*	pvTemp;
	M*	pElement1;
	M*	pElement2;

	//Only allocate memory if we don't alreay have temporay memory assigned.
	if (miUsedElements != miUsedElements)
	{
		pvTemp = (M*)((size_t)pvArray + (miUsedElements-1) * miElementSize);
	}
	else
	{
		pvTemp = (M*)Malloc(miElementSize);
	}

	pElement1 = (M*)((size_t)pvArray + (iElement1) * miElementSize);
	pElement2 = (M*)((size_t)pvArray + (iElement2) * miElementSize);
	memcpy(pvTemp, pElement1, miElementSize);
	memcpy(pElement1, pElement2, miElementSize);
	memcpy(pElement2, pvTemp, miElementSize);

	//Free any memory we had to allocate.
	if (miUsedElements == miNumElements)
	{
		Free(pvTemp);
	}
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
//template<class M>
//void __CArrayTemplate<M>::QuickSort(int(__cdecl* Func)(const void*, const void*))
//{
//	qsort((void*)pvArray, miUsedElements, miElementSize, Func);
//}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
/*
template<class M>
void __CArrayTemplate<M>::BubbleSort(int(__cdecl* Func)(const void*, const void*))
{
	int		iPos;
	int		iResult;
	BOOL	bSwapped;
	int		iNumDone;

	bSwapped = TRUE;
	iNumDone = 0;
	while (bSwapped)
	{
		iPos = 0;
		bSwapped = FALSE;

		while ((iPos+1) < (miUsedElements - iNumDone))
		{
			iResult = Func(Get(iPos), Get(iPos+1));
			if (iResult > 0)
			{
				Swap(iPos, iPos+1);
				bSwapped = TRUE;
			}
			iPos++;
		}
		iNumDone++;
	}
}
*/

//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
/*
template<class M>
void __CArrayTemplate<M>::InsertIntoSorted(int(__cdecl* Func)(const void*, const void*), M* pvElement)
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
int __CArrayTemplate<M>::FindElementInSorted(M* pData, int(__cdecl* Func)(const void*, const void*), int bFindIfNone)
{
	if (miUsedElements == 0)
	{
		if (bFindIfNone)
		{
			return 0;
		}
		return -1;
	}
	return 0;
//	return BinarySearch(pData, 0, miUsedElements - 1, Func, bFindIfNone);
}*/


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
//template<class M>
//int __CArrayTemplate<M>::BinarySearch(M* pData, int iLeft, int iRight, int(__cdecl* Func)(const void*, const void*), int bFindIfNone)
//{
//	int		iMiddle;
//	int		iResultMiddle;
//
//	iResultMiddle = 0;
//	while (iLeft <= iRight)
//	{
//		iMiddle = (iLeft + iRight) >> 1; //Divide by 2
//		iResultMiddle = Func(pData, Get(iMiddle));
//		if (iResultMiddle == 0)
//		{
//			return iMiddle;
//		}
//		else if (iResultMiddle < 0)
//		{
//			iRight = iMiddle - 1;
//		}
//		else
//		{
//			iLeft = iMiddle + 1;
//		}
//	}
//
//	if (bFindIfNone)
//	{
//		if (iResultMiddle > 0)
//		{
//			return iMiddle + 1;
//		}
//		return iMiddle;
//	}
//	return -1;  
//}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
void __CArrayTemplate<M>::CopyArrayInto(__CArrayTemplate<M>* pcTemplateArray, int iElementPos)
{
	memcpy(Get(iElementPos), pcTemplateArray->pvArray, miElementSize * pcTemplateArray->miUsedElements);
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
void __CArrayTemplate<M>::InsertArrayAfterEnd(__CArrayTemplate<M>* pcTemplateArray)
{
	int		iArrayPos;

	iArrayPos = GrowByNumElements(pcTemplateArray->miUsedElements);
	CopyArrayInto(pcTemplateArray, iArrayPos);
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
void __CArrayTemplate<M>::InsertArrayBeforeStart(__CArrayTemplate<M>* pcTemplateArray)
{
	InsertNumElementsAt(pcTemplateArray->NumElements(), 0);
	CopyArrayInto(pcTemplateArray, 0);
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
void __CArrayTemplate<M>::InsertArrayAt(__CArrayTemplate<M>* pcTemplateArray, int iElementPos)
{
	InsertNumElementsAt(pcTemplateArray->NumElements(), iElementPos);
	CopyArrayInto(pcTemplateArray, iElementPos);
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
M* __CArrayTemplate<M>::InsertNumElementsAt(int iNumElements, int iElementPos)
{
	void*	pvFrom;
	void*	pvTo;
	int		iNumToMove;

	iNumToMove = miUsedElements - iElementPos;
	GrowByNumElements(iNumElements);

	pvFrom = (void*)Get(iElementPos);
	pvTo = (void*)Get(iElementPos + iNumElements);
	memmove(pvTo, pvFrom, iNumToMove * miElementSize);
	return (M*)pvFrom;
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
int __CArrayTemplate<M>::MallocSize(void)
{
	return ByteSize();
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
M* __CArrayTemplate<M>::Get(int iElementPos)
{
	return (M*)(size_t)((int)(size_t)pvArray + miElementSize * iElementPos);
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
int __CArrayTemplate<M>::ElementSize(void)
{
	return miElementSize;
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
int __CArrayTemplate<M>::NumElements(void)
{
	return miUsedElements;
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
int __CArrayTemplate<M>::GetAdjustedPosition(int iIndex)
{
	if (iIndex == -1)
	{
		return NumElements()-1;
	}
	return iIndex;
}

//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
void CArrayTemplate<M>::Init(void)
{
	this->miElementSize = sizeof(M);
	this->miNumElements = 0;
	this->miUsedElements = 0;
	this->miChunkSize = 1;
	this->pvArray = NULL;
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
void CArrayTemplate<M>::Init(int iChunkSize)
{
	Init();
	this->SetAllocateSize(iChunkSize);
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
void CArrayTemplate<M>::Allocate(int iChunkSize)
{
	Init(iChunkSize);
	this->GrowByChunk();
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
template<class M>
void CArrayTemplate<M>::Allocate(int iChunkSize, int iNumElements)
{
	this->miElementSize = sizeof(M);
	this->miChunkSize = iChunkSize;
	this->pvArray = NULL;
	this->SetUsedElements(iNumElements);
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
template<class M>
void CArrayTemplate<M>::ReInit(void)
{
	this->Kill();
	this->Init(this->miChunkSize);
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
template<class M>
M& CArrayTemplate<M>::operator[](int iElementPos)
{
	return 	this->pvArray[iElementPos];
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
M* CArrayTemplate<M>::Get(int iElementPos)
{
	return &	this->pvArray[iElementPos];
}


#endif

