#include "ArrayInt.h"


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
void CArrayInt::Add(int iElement)
{
	__CArrayInt::Add(&iElement);
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
void CArrayInt::QuickSort(void)
{
	qsort(pvArray, miUsedElements, sizeof(int), CompareInt);
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////

void CArrayInt::SetArrayValues(int iValue)
{
	for (int i = 0; i < miUsedElements; i++)
	{
		pvArray[i] = iValue;
	}
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
int CArrayInt::FindElement(int iValue)
{
	for (int i = 0; i < miUsedElements; i++)
	{
		if (pvArray[i] == iValue)
		{
			return i;
		}
	}
	return -1;
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
int CArrayInt::AddIfUnique(int iValue)
{
	int iPos;

	iPos = FindElement(iValue);
	if (iPos == -1)
	{
		Add(iValue);
	}
	return iPos;
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
void CArrayInt::AddRemap(int iElementPos, int iRemapNum)
{
	int*	pi;
	int		i;
	int		iNumToAdd;
	int		iMinusOne;

	if (iElementPos >= 0)
	{
		if (miUsedElements > iElementPos)
		{
			pi = (int*)CArrayTemplate<int>::Get(iElementPos);
			(*pi) = iRemapNum;
		}
		else
		{
			iMinusOne = -1;
			iNumToAdd = (iElementPos - miUsedElements);
			
			for (i = 0; i < iNumToAdd; i++)
			{
				Add(iMinusOne);
			}
			Add(iRemapNum);
		}
	}
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
void CArrayInt::RemoveRemap(int iElementPos)
{
	int*	pi;
	int		i;
	int		iCutDown;

	if (iElementPos < miUsedElements)
	{
		pi = (int*)CArrayTemplate<int>::Get(iElementPos);
		(*pi) = -1;
	}

	iCutDown = miUsedElements;
	for (i = miUsedElements-1; i >= 0; i--)
	{
		pi = (int*)CArrayTemplate<int>::Get(iElementPos);
		if (*pi == -1)
		{
			iCutDown = i;
		}
		else
		{
			break;
		}
	}
	if (iCutDown != miUsedElements)
	{
		SetUsedElements(iCutDown);
	}
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
void CArrayInt::Swap(int iElement1, int iElement2)
{
	int*	piElement1;
	int*	piElement2;
	int		iTemp;

	piElement1 = (int*)((size_t)pvArray + iElement1 * sizeof(int));
	piElement2 = (int*)((size_t)pvArray + iElement2 * sizeof(int));
	iTemp = (*piElement1);
	(*piElement1) = (*piElement2);
	(*piElement2) = iTemp;
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
int CompareInt(const void* arg1, const void* arg2)
{
	if ((*((int*)arg1)) < (*((int*)arg2)))
	{
		return -1;
	}
	if ((*((int*)arg1)) > (*((int*)arg2)))
	{
		return 1;
	}
	return 0;
}

