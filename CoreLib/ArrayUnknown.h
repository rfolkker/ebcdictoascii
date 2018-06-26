#ifndef __ARRAY_UNKNOWN_H__
#define __ARRAY_UNKNOWN_H__
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include "ArrayTemplate.h"


typedef __CArrayTemplate<void> __CArrayUnknown;
class CArrayUnknown : public __CArrayUnknown
{
public:
	int		miElementType;

	void	Init(int iSize, int iType);
	void	Init(int iSize, int iType, int iChunkSize);
	void 	Allocate(int iSize, int iType, int iChunkSize);
	void 	Allocate(int iSize, int iType, int iChunkSize, int iNumElements);
	void 	ReInit(void);
};


#endif

