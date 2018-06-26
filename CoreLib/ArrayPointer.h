#ifndef __ARRAY_POINTER_H__
#define __ARRAY_POINTER_H__
#include "ArrayTemplate.h"


//This structure is in the wrong place and should also be use in the EnumeratorPointer.
struct STypedPointer
{
	void*	pvData;
	int		iType;  //Arbitrary type values... no need to be a registered type.
};


typedef CArrayTemplate<STypedPointer> __CArrayPointer;
class CArrayPointer : public __CArrayPointer
{
public:
	void 	Add(void* pvData, int iType);
	void 	Get(int iElementPos, void** pvData, int* iType);
	void*	GetPtr(int iElementPos);
	void*	SafeGetPtr(int iElementPos);
	int		GetType(int iElementPos);
	int		SafeGetType(int iElementPos);
};


#endif

