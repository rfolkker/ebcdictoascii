#ifndef __TEMPLATE_LIST_H__
#define __TEMPLATE_LIST_H__
#include <stdlib.h>
#include "DataTypes.h"


template<class M>
class CLinkListTemplate
{
protected:	
	void*	Malloc(int iMemSize);
	void	Free(void* pvMem);

public:
	SDNode*		mpsHead; 
	SDNode*		mpsTail;
	int			miNumElements;

	void	Init(void);
	void	Kill(void);
	M* 		GetHead(void);
	M* 		GetTail(void);
	M* 		GetNext(M* pvData);
	M* 		GetPrev(M* pvData);
	M* 		InsertAfterTail(void);
	M* 		InsertBeforeHead(void);
	void	InsertDetachedAfterTail(M* psNode);
	void	InsertDetachedBeforeHead(M* psNode);
	M*		InsertBeforeNode(M* psPos);
	M*		InsertAfterNode(M* psPos);
	void	InsertDetachedBeforeNode(M* psData, M* psPos);
	void	InsertDetachedAfterNode(M* psData, M* psPos);
	void	Remove(M* pvData);
	void	Detach(SDNode* psNodeHeader);
	void	Detach(M* pvData);
	void	FreeDetached(M* pvData);
	M*		AllocateDetached(void);
	int		GetNumElements(void);

	//Link list insertion functions.
void InsertLinkListAfterTail(CLinkListTemplate<M>* pcLinkList);
void InsertLinkListBeforeHead(CLinkListTemplate<M>* pcLinkList);
void InsertLinkListAfterNode(CLinkListTemplate<M>* pcLinkList, M* psPos);
void InsertLinkListBeforeNode(CLinkListTemplate<M>* pcLinkList, M* psPos);

	//Sorting functions.
	void	Swap(M* psData1, M* psData2);
//	void	BubbleSort(int(const M*, const M*));
//	void QuickSort(int(const M*, const M*));
//	void	InsertIntoSorted(int(const M*, const M*), M* psNode);

	//Get malloc size.
	int MallocSize(void);
};


#define CLinkListTemplateDataGetHeader(pvData)		DataGetHeaderMacro<SDNode, M>(pvData)
#define CLinkListTemplateHeaderGetData(pvHeader)	HeaderGetDataMacro<SDNode, M>(pvHeader)


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
void* CLinkListTemplate<M>::Malloc(int iMemSize)
{
	return malloc(iMemSize);
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
void CLinkListTemplate<M>::Free(void* pvMem)
{
	free(pvMem);
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
void CLinkListTemplate<M>::Init(void)
{
	mpsHead = NULL;
	mpsTail = NULL;
	miNumElements = 0;
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
void CLinkListTemplate<M>::Kill(void)
{
	SDNode*	psNode;
	SDNode*	psNode2;

	psNode = mpsHead;
	while (psNode)
	{
		psNode2 = psNode->psNext;
		Free(psNode);
		psNode = psNode2;
	}
	mpsHead = NULL;
	mpsTail = NULL;
	miNumElements = 0;
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
M* CLinkListTemplate<M>::GetHead(void)
{
	return CLinkListTemplateHeaderGetData(mpsHead);
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
M* CLinkListTemplate<M>::GetTail(void)
{
	return CLinkListTemplateHeaderGetData(mpsTail);
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
M* CLinkListTemplate<M>::GetNext(M* pvData)
{
	SDNode*		psNodeHeader;

	psNodeHeader = CLinkListTemplateDataGetHeader(pvData);
	if (psNodeHeader->psNext)
	{
		return CLinkListTemplateHeaderGetData(psNodeHeader->psNext);
	}
	return NULL;
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
M* CLinkListTemplate<M>::GetPrev(M* pvData)
{
	SDNode*		psNodeHeader;

	psNodeHeader = CLinkListTemplateDataGetHeader(pvData);
	if (psNodeHeader->psPrev)
	{
		return CLinkListTemplateHeaderGetData(psNodeHeader->psPrev);
	}
	return NULL;
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
M* CLinkListTemplate<M>::InsertAfterTail(void)
{
	SDNode*		psNode;
	M*			psData;
	
	psNode = (SDNode*)Malloc(sizeof(SDNode) + sizeof(M));
	psData = CLinkListTemplateHeaderGetData(psNode);
	InsertDetachedAfterTail(psData);
	return psData;
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
M* CLinkListTemplate<M>::InsertBeforeHead(void)
{
	SDNode*		psNode;
	M*			psData;

	psNode = (SDNode*)Malloc(sizeof(SDNode) + sizeof(M));
	psData = CLinkListTemplateHeaderGetData(psNode);
	InsertDetachedBeforeHead(psData);
	return psData;
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
M* CLinkListTemplate<M>::InsertBeforeNode(M* psPos)
{
	SDNode*		psNode;
	M*			psData;

	psNode = (SDNode*)Malloc(sizeof(SDNode) + sizeof(M));
	psData = CLinkListTemplateHeaderGetData(psNode);
	InsertDetachedBeforeNode(psData, psPos);
	return psData;
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
M* CLinkListTemplate<M>::InsertAfterNode(M* psPos)
{
	SDNode*		psNode;
	M*			psData;

	psNode = (SDNode*)Malloc(sizeof(SDNode) + sizeof(M));
	psData = CLinkListTemplateHeaderGetData(psNode);
	InsertDetachedAfterNode(psData, psPos);
	return psData;
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
void CLinkListTemplate<M>::InsertDetachedAfterTail(M* psData)
{
	SDNode*		psNode;

	psNode = CLinkListTemplateDataGetHeader(psData);
	psNode->psNext = NULL;
	psNode->psPrev = mpsTail;

	if (mpsHead == NULL)
	{
		mpsHead = psNode;
	}
	else
	{
		mpsTail->psNext = psNode;
	}
	mpsTail = psNode;
	miNumElements++;
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
void CLinkListTemplate<M>::InsertDetachedBeforeHead(M* psData)
{
	SDNode*		psNode;

	psNode = CLinkListTemplateDataGetHeader(psData);
	psNode->psNext = mpsHead;
	psNode->psPrev = NULL;

	if (mpsTail == NULL)
	{
		mpsTail = psNode;
	}
	else
	{
		mpsHead->psPrev = psNode;
	}
	mpsHead = psNode;
	miNumElements++;
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
void CLinkListTemplate<M>::InsertDetachedBeforeNode(M* psDataNode, M* psDataPos)
{
	SDNode*		psPos;
	SDNode*		psNode;

	psNode = CLinkListTemplateDataGetHeader(psDataNode);
	psPos = CLinkListTemplateDataGetHeader(psDataPos);

	psNode->psPrev = psPos->psPrev;
	psNode->psNext = psPos;

	if (psPos->psPrev)
	{
		psPos->psPrev->psNext = psNode;
	}
	else
	{
		mpsHead = psNode;
	}
	psPos->psPrev = psNode;
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
void CLinkListTemplate<M>::InsertDetachedAfterNode(M* psDataNode, M* psData)
{
	SDNode*		psPos;
	SDNode*		psNode;

	psNode = CLinkListTemplateDataGetHeader(psDataNode);
	psPos = CLinkListTemplateDataGetHeader(psData);

	psNode->psNext = psPos;
	psNode->psPrev = psPos->psNext;

	if (psPos->psNext)
	{
		psPos->psNext->psPrev = psNode;
	}
	else
	{
		mpsTail = psNode;
	}
	psPos->psNext = psNode;
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
void CLinkListTemplate<M>::Remove(M* psNodeData)
{	
	Detach(psNodeData);
	FreeDetached(psNodeData);
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
void CLinkListTemplate<M>::Detach(SDNode* psNodeHeader)
{
	if (psNodeHeader->psPrev)
	{
		psNodeHeader->psPrev->psNext = psNodeHeader->psNext;
	}
	else
	{
		mpsHead = psNodeHeader->psNext;
	}
	if (psNodeHeader->psNext)
	{
		psNodeHeader->psNext->psPrev = psNodeHeader->psPrev;
	}
	else
	{
		mpsTail = psNodeHeader->psPrev;
	}
	miNumElements--;
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
void CLinkListTemplate<M>::Detach(M* psNodeData)
{
	SDNode*		psNodeHeader;

	psNodeHeader = CLinkListTemplateDataGetHeader(psNodeData);
	Detach(psNodeHeader);
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
void CLinkListTemplate<M>::FreeDetached(M* psNodeData)
{
	SDNode*		psNodeHeader;

	psNodeHeader = CLinkListTemplateDataGetHeader(psNodeData);
	if (psNodeHeader)
	{
		Free(psNodeHeader);
	}
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
M* CLinkListTemplate<M>::AllocateDetached(void)
{
	SDNode*		psNode;
	
	psNode = (SDNode*)Malloc(sizeof(SDNode) + sizeof(M));
	return CLinkListTemplateHeaderGetData(psNode);
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
void CLinkListTemplate<M>::Swap(M* psData1, M* psData2)
{
	SDNode*		psNode1;
	SDNode*		psNode2;
	SDNode		sNodeTemp1;
	SDNode		sNodeTemp2;

	psNode1 = CLinkListTemplateDataGetHeader(psData1);
	psNode2 = CLinkListTemplateDataGetHeader(psData2);

	//Swapping a node for NULL is stupid
	if ((psNode1 == NULL) || (psNode2 == NULL) || (psNode1 == psNode2))
	{
		return;
	}
	
	sNodeTemp1.psNext = psNode1->psNext;
	sNodeTemp1.psPrev = psNode1->psPrev;
	sNodeTemp2.psNext = psNode2->psNext;
	sNodeTemp2.psPrev = psNode2->psPrev;

	if ((psNode1->psNext != psNode2) && (psNode1->psPrev != psNode2))
	{
		//Nodes to be swapped do not point at each other

		if (sNodeTemp1.psPrev)	{ sNodeTemp1.psPrev->psNext = psNode2; } else { mpsHead = psNode2; }
		if (sNodeTemp1.psNext)	{ sNodeTemp1.psNext->psPrev = psNode2; } else { mpsTail = psNode2; }
		if (sNodeTemp2.psPrev)	{ sNodeTemp2.psPrev->psNext = psNode1; } else { mpsHead = psNode1; }
		if (sNodeTemp2.psNext)	{ sNodeTemp2.psNext->psPrev = psNode1; } else { mpsTail = psNode1; }

		psNode1->psNext = sNodeTemp2.psNext;
		psNode1->psPrev = sNodeTemp2.psPrev;
		psNode2->psNext = sNodeTemp1.psNext;
		psNode2->psPrev = sNodeTemp1.psPrev;
	} 
	else if (psNode1->psNext==psNode2)
	{
		if (sNodeTemp1.psPrev)	{ sNodeTemp1.psPrev->psNext=psNode2; } else { mpsHead=psNode2; }
		if (sNodeTemp2.psNext)	{ sNodeTemp2.psNext->psPrev=psNode1; } else { mpsTail=psNode1; }

		psNode1->psNext = sNodeTemp2.psNext;
		psNode1->psPrev = psNode2;
		psNode2->psNext = psNode1;
		psNode2->psPrev = sNodeTemp1.psPrev;
	}
	else if (psNode1->psPrev==psNode2)
	{
		if (sNodeTemp1.psNext)	{ sNodeTemp1.psNext->psPrev=psNode2; } else { mpsTail=psNode2; }
		if (sNodeTemp2.psPrev)	{ sNodeTemp2.psPrev->psNext=psNode1; } else { mpsHead=psNode1; }

		psNode1->psNext = psNode2;
		psNode1->psPrev = sNodeTemp2.psPrev;
		psNode2->psNext = sNodeTemp1.psNext;
		psNode2->psPrev = psNode1;
	}
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
int	CLinkListTemplate<M>::GetNumElements(void)
{
	return miNumElements;
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
//
//	!!!!!!!!!! CAUTION: Not optimised !!!!!!!!!!
//
/*
template<class M>
void CLinkListTemplate<M>::BubbleSort(int(__cdecl* Func)(const M*, const M*))
{
	M*		pcCurr;
	M*		pcNext;
	int		iResult;
	BOOL	bSwapped;

	bSwapped = TRUE;
	while (bSwapped)
	{
		pcCurr = GetHead();
		pcNext = GetNext(pcCurr);
		bSwapped = FALSE;

		while ((pcNext) && (pcCurr))
		{
			iResult = Func(pcCurr, pcNext);
			if (iResult > 0)
			{
				Swap(pcCurr, pcNext);
				bSwapped = TRUE;
			}
			else
			{
				pcCurr = GetNext(pcCurr);
			}
			if (pcCurr)
			{
				pcNext = GetNext(pcCurr);
			}
		}
	}
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
void CLinkListTemplate<M>::QuickSort(int(__cdecl*)(const M*, const M*))
{
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
void CLinkListTemplate<M>::InsertIntoSorted(int(__cdecl* Func)(const M*, const M*), M* psData)
{
	M*			pcCurr;
	int			iResult;

	pcCurr = GetHead();

	for(;;)
	{
		iResult = Func(psData, pcCurr);
		if (iResult < 0)
		{
			InsertDetachedBeforeNode(psData, pcCurr);
			break;
		}
		pcCurr = GetNext(pcCurr);
		if (pcCurr == NULL)
		{
			InsertDetachedAfterTail(psData);
			break;
		}
	}
}
*/

//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
void CLinkListTemplate<M>::InsertLinkListAfterTail(CLinkListTemplate<M>* pcLinkList)
{
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
void CLinkListTemplate<M>::InsertLinkListBeforeHead(CLinkListTemplate<M>* pcLinkList)
{
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
void CLinkListTemplate<M>::InsertLinkListAfterNode(CLinkListTemplate<M>* pcLinkList, M* psPos)
{
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
void CLinkListTemplate<M>::InsertLinkListBeforeNode(CLinkListTemplate<M>* pcLinkList, M* psPos)
{
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class M>
int CLinkListTemplate<M>::MallocSize(void)
{
	return ((sizeof(SDNode) + sizeof(M)) * miNumElements);
}


#endif

