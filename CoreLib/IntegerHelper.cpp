/*
//////////////////////////////////////////////////////////////////////////
//
//  Returns the Nearest power of two to this number rounded down.
//	eg: 1025 become 1024 but 1024 becomes 1024.  (also 2047 becomes 1024).
//
//////////////////////////////////////////////////////////////////////////
__inline int TruncateLowBits(int i)
{
	_asm {
		mov eax, i
		mov ebx, 0x80000000

TLBTestLoop:
		test eax, ebx
		jnz TLBTestDone
		shr ebx, 1
		jnz TLBTestLoop
		jmp TLBComplete

TLBTestDone:
		shr ebx, 1
		jz TLBComplete
		not ebx

TLBAndLoop:
		and eax, ebx
		stc
		ror ebx, 1
		cmp ebx, 0x7fffffff
		jnz TLBAndLoop

TLBComplete:
	}

	//The answer is returned in eax.
}


//////////////////////////////////////////////////////////////////////////
//
//	Count how many bits are not used in zero extending a DWORD.
//	eg: 00011111 becomes 4 and 00010000 also becomes 4.
//
//////////////////////////////////////////////////////////////////////////
__inline int GetLogBase2(int i)
{
	__asm
	{
		mov ecx, 31
		mov eax, i 
GNBAgain:
		mov ebx, eax
		and ebx, 0x80000000
		cmp ebx, 0x80000000
		jz GNBFoundIt
		dec ecx
		jz GNBFoundIt
		shl eax, 1
		jmp GNBAgain
GNBFoundIt:
		mov eax, ecx
	}
}


//////////////////////////////////////////////////////////////////////////
//
//  Returns the Nearest power of two to this number rounded up.
//	eg: 1025 become 2048 but 1024 becomes 1024.  (also 1023 becomes 1024).
//
//////////////////////////////////////////////////////////////////////////
__inline int GetBestHighBit(int i)
{
	int i2;

	i2 = i;
	i = TruncateLowBits(i);
	__asm {
		mov eax, i
		cmp eax, i2
		jz GBHBComplete
		shl eax, 1
GBHBComplete:
	}
}


//////////////////////////////////////////////////////////////////////////
//
//  Returns the byte at position ipos in a DWORD.  Useful for colours.
//	eg:  iInt = 0x00abcdef, iPos = 1 returns 0x000000cd.
//	eg:  iInt = 0x00abcdef, iPos = 2 returns 0x000000ab.
//
//////////////////////////////////////////////////////////////////////////
__inline int GetByte(int iInt, int iPos)
{
	int c;

	iPos *= 8;
	iInt >>= iPos;
	c = (unsigned char)(iInt & 0xff);
	return c;
}


//////////////////////////////////////////////////////////////////////////
//
//	Same as above but sets the byte.
//
//////////////////////////////////////////////////////////////////////////
__inline int SetByte(int c, int iPos)
{
	iPos *= 8;
	if (c > 256)
	{
		c = 256;
	}
	c <<= iPos;
	return c;
}


//////////////////////////////////////////////////////////////////////////
//
//	Gets the state of a bit in an array given the bit position.
//	No bounds checking is done.
//
//////////////////////////////////////////////////////////////////////////
__inline char GetBit(int iBit, void* pvArray)
{
	__asm
	{
		mov eax, iBit
		xor edx, edx
		mov ebx, 32
		div ebx  //now eax contains the DWORD position and edx the bit position

		mov edi, pvArray
		shl eax, 2
		mov ecx, [edi + eax]  //Now ecx contains the DWORD we're dealing with.
		xchg ecx, edx  //Now edx contains DWORD and ecx contains BIT.

		bt edx, ecx
		setc al 
		and eax, 0x1
	}
}


//////////////////////////////////////////////////////////////////////////
//
//	Sets the state of a bit in an array given the bit position and state.
//	No bounds checking is done.
//
//////////////////////////////////////////////////////////////////////////
__inline void SetBit(int iBit, void* pvArray, char bBit)
{
	__asm
	{
		mov eax, iBit
		xor edx, edx
		mov ebx, 32
		div ebx  //now eax contains the DWORD position and edx the bit position

		mov edi, pvArray
		shl eax, 2
		mov ebx, eax
		mov ecx, [edi + ebx]  //Now ecx contains the DWORD we're dealing with.
		xchg ecx, edx  //Now edx contains DWORD and ecx contains BIT.

		mov eax, 1
		shl eax, cl
		not eax
		and edx, eax
		mov ch, bBit
		cmp ch, 0
		jz SBNoChange
		not eax
		or edx, eax
SBNoChange:
		mov [edi + ebx], edx  //Now ecx contains the DWORD we're dealing with.
	}
}


//////////////////////////////////////////////////////////////////////////
//
//	iArray size is in bits
//
//////////////////////////////////////////////////////////////////////////
__inline int FindFirstSetBit(void* pvArray, int iArraySize)
{
	__asm
	{
		mov edi, pvArray
		xor eax, eax  //Total bit position
		xor ebx, ebx  //DWORD position
FFCBScan:
		bsf ecx, [edi]  //Find first SET bit.
		jz FFCBZero	
		add eax, ecx
		cmp eax, iArraySize
		jl FFCBExit
		jmp FFCBNoClearBit
FFCBZero:
		add eax, 32
		cmp eax, iArraySize
		jge FFCBNoClearBit
		add edi, 4
		jmp FFCBScan
FFCBNoClearBit:
		mov eax, -1
FFCBExit:
	}
}


//////////////////////////////////////////////////////////////////////////
//
//	iArray size is in bits
//
//////////////////////////////////////////////////////////////////////////
__inline int FindFirstClearBit(void* pvArray, int iArraySize)
{
	__asm
	{
		mov edi, pvArray
		xor eax, eax  //Total bit position
		xor ebx, ebx  //DWORD position
FFSBScan:
		mov edx, [edi]
		not edx
		bsf ecx, edx  //Find first NOT SET bit.
		jz FFSBZero	
		add eax, ecx
		cmp eax, iArraySize
		jl FFSBExit
		jmp FFSBNoClearBit
FFSBZero:
		add eax, 32
		cmp eax, iArraySize
		jge FFSBNoClearBit
		add edi, 4
		jmp FFSBScan
FFSBNoClearBit:
		mov eax, -1
FFSBExit:
	}
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
__inline int FixBool(void* pv)
{
	return FixBool((int)(size_t)pv);
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
__inline int FixBool(int i)
{
	__asm 
	{
		mov eax, i 
		cmp eax, 0
		jz FBZero
		mov eax, 1
FBZero:
	}
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
__inline void SetFlag(int* piDest, int iFlag, int iFlagValue)
{
	//If the value is true then or it with dest.
	if (iFlagValue)
	{
		*piDest |= iFlag;
	}
	//If the value is false then negate and and it with dest.
	else
	{
		*piDest &= (~iFlag);
	}
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
__inline void Swap(int* i1, int* i2)
{
	int i3;

	i3 = *i1;
	*i1 = *i2;
	*i2 = i3;

	//__asm
	//{
	//	//You can possibly use xchg here...
	//}
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
__inline int FindFirstInt(int* piIntArray, int iSearch, int iMaxLength)
{
	for (int i = 0; i < iMaxLength; i++)
	{
		if (piIntArray[i] == iSearch)
		{
			return i;
		}
	}
	return -1;
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
*/
int FindFirstByte(char* pcCharArray, char cSearch, int iMaxLength)
{
	for(int i=0; i<iMaxLength; i++)
	{
		if(pcCharArray[i] == cSearch)
		{
			return i;
		}
	}
	return -1;
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
int GetHighNibble(char c)
{
	return ((int)c >> 4) & 0xf;
}


//////////////////////////////////////////////////////////////////////////
//
//
//////////////////////////////////////////////////////////////////////////
int GetLowNibble(char c)
{
	return (int)c & 0xf;
}
