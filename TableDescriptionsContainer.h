#ifndef __TABLE_DESCRIPTIONS_CONTAINER_H__
#define __TABLE_DESCRIPTIONS_CONTAINER_H__
#include "TableDescription.h"


typedef CArrayTemplate<CTableDescription>  CArrayCTableDescription;


class CTableDescriptionsContainer
{
public:
	CArrayCTableDescription	tableDescriptions;
	CString								mcLibrary;
	
	void 								Init(void);
	void 								Kill(void);
	BOOL 								AddLibrary(char* szTableDesc, char* szLibrary);
	CTableDescription*	Get(char* szTableName);
	CTableDescription*	Add(CString Name, CString source);
};


#endif 

