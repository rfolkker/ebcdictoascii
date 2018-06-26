#ifndef __INTEGER_HELPER_H__
#define __INTEGER_HELPER_H__


#define MAXINT ((1 << 31) - 1)
#define MININT (1 << 31)

/*
int TruncateLowBits(int i);
int GetBestHighBit(int i);
int GetLogBase2(int i);
int GetByte(int iInt, int iPos);
int SetByte(int c, int iPos);
char GetBit(int iBit, void* pvArray);  //return 1 or 0 in char.
void SetBit(int iBit, void* pvArray, char bBit);  //bBit can only take 1 or 0.
void SetFlag(int* piDest, int iFlag, int iFlagValue);  //Identically to set bit but only works on 4bytes.
int FindFirstClearBit(void* pvArray, int iArraySize);
int FindFirstSetBit(void* pvArray, int iArraySize);
int FixBool(int i);
int FixBool(void* pv);
void Swap(int i1, int i2);
int FindFirstInt(int* piIntArray, int iSearch, int iMaxLength);
*/
int	GetLowNibble(char c);
int	GetHighNibble(char c);
int	FindFirstByte(char* pcCharArray, char cSearch, int iMaxLength);

//#include "IntegerHelper.inl"


#endif


