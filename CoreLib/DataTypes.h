#ifndef __DATA_TYPES_H__
#define __DATA_TYPES_H__


//For the LinkedList.
struct SDNode
{
	SDNode*		psNext;
	SDNode*		psPrev;
};


//For runtime-known data times.
struct SUnknownType
{
	int			miType;
	int			miSize;
};


struct SDUNode
{
	SUnknownType	sType;
	SDNode			sDNode;
};


//For the Tree.
struct STNode
{
	STNode*		psUp;
	STNode*		psAcross;	
	STNode*		psDown;
};


struct STUNode
{
	SUnknownType	sType;
	STNode			sTNode;
};


//For the enumerator.
struct SENode
{
	int			iNum;
	int			iFlags;
	void*		pvData;
	char*		szName;
	int			iDataSize;
};


//For the Freelist.
struct SFNode
{
	int			iFull;
};


void* UnknownTypeAllocate(int iSize, int iType);
int UnknownTypeGetType(void* pvData);
int UnknownTypeGetSize(void* pvData);
void UnknownTypeFree(void* pvData);


#define HeaderAndDataAllocate(SHeaderType, SDataType) \
	HeaderAllocateMacro<SHeaderType, SDataType>()

#define HeaderGetData(SHeaderType, SDataType, Header) \
	HeaderGetDataMacro<SHeaderType, SDataType>(Header)

#define DataGetHeader(SHeaderType, SDataType, Data) \
	DataGetHeaderMacro<SHeaderType, SDataType>(Data)

#define HeaderAndDataFree(SHeaderType, SDataType, Data) \
	HeaderFreeMacro<SHeaderType, SDataType>(Data)


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class T, class M>
M* HeaderAllocateMacro()
{
	void*	pv;

	pv = malloc(sizeof(T) + sizeof(M));
	if (pv == NULL)
	{
		return NULL;
	}
	return (M*)(((int)(pv) + sizeof(T)));
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class T, class M>
M* HeaderGetDataMacro(T* pv)
{
	if (pv == NULL)
	{
		return NULL;
	}
	return (M*)(((int)(pv) + sizeof(T)));
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class T, class M>
T* DataGetHeaderMacro(M* pv)
{
	if (pv == NULL)
	{
		return NULL;
	}
	return (T*)(((int)(pv) - sizeof(T)));
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
template<class T, class M>
void HeaderFreeMacro(M* pv)
{
	T* pvHeader;

	pvHeader = DataGetHeaderMacro<T, M>(pv);
	if (pvHeader)
	{
		free(pvHeader);
	}
}

#endif

