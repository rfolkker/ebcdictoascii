#include "EnumeratorTemplate.h"


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
int AlphabeticalComparisionCallbackCaseSensitive(const SENode* psNode1, const SENode* psNode2)
{
	if ((!psNode1->szName) || (!psNode2->szName))
	{
		return 0;
	}

	return strcmp(psNode1->szName, psNode2->szName);
}


//////////////////////////////////////////////////////////////////////////
//																		//
//																		//
//////////////////////////////////////////////////////////////////////////
/*
int AlphabeticalComparisionCallbackCaseInsensitive(const SENode* psNode1, const  SENode* psNode2)
{
	if ((!psNode1->szName) || (!psNode2->szName))
	{
		return 0;
	}

	return stricmp(psNode1->szName, psNode2->szName);
}
*/
