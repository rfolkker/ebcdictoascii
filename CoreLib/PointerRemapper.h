#ifndef __POINTER_REMAPPER_H__
#define __POINTER_REMAPPER_H__


void*	RemapSinglePointer(void* pvPtr, int iOffest);
__inline void* RemapSinglePointer(void* pvPtr, int iOffest)
{
	return &((char*)pvPtr)[iOffest];
}


#endif

