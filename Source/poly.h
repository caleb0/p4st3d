/*
*   Copyright � May-29-2010 by learn_more
*   SimpleObf.cpp is part of the project 'SimpleObfuscation'.
*
*   Please do not use this in payhacks.
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY, without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*
*$UCC_HDR$*
*/


/******************************************************************************\
**                                                                            **
**     Please do not edit this file, unless you know what you are doing!      **
**                                                                            **
\******************************************************************************/


#define SIMPLE_OBF_CPP_VERSION		003

#define _WIN32_WINNT 0x0501
#define _WIN32_WINDOWS 0x0410
#define _WIN32_IE 0x0600
#include "windows.h"
/*
*   Copyright � May-30-2010 by learn_more
*   SimpleObf.h is part of the project 'SimpleObfuscation'.
*
*   Please do not use this in payhacks.
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY, without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*
*$UCC_HDR$*
*/

#define SIMPLE_OBF_H_VERSION		003

/******************************************************************************\
**                                                                            **
**                              Expansion macros                              **
**                      not recommended for direct usage                      **
**                                                                            **
\******************************************************************************/

#define PASTE(x,y)	x##y

#define OBFUSCATENOPS3(x,obf_start_nops,obf_end_nops) \
obf_start_nops: \
	x \
	__asm { \
	__asm push obf_end_nops \
	__asm push obf_start_nops \
	__asm call _AddNops \
} \
obf_end_nops: 

#define DEL_START3(del_start_lbl) \
del_start_lbl:

#define DEL_END3(del_start_lbl,del_end_lbl) \
	__asm { \
	__asm push del_end_lbl \
	__asm push del_start_lbl \
	__asm call _AddNops \
} \
del_end_lbl: 


#define OBFUSCATENOPS2(x,y)	OBFUSCATENOPS3(x,PASTE(obf_start_nops_,y),PASTE(obf_end_nops_,y))

#define DEL_START2(x,y)	DEL_START3(PASTE2(del_start_lbl_,x,y))
#define DEL_END2(x,y)	DEL_END3(PASTE2(del_start_lbl_,x,y),PASTE2(del_end_lbl_,x,y))



/******************************************************************************\
**                                                                            **
**                    Macros / functions for direct usage                     **
**                                                                            **
\******************************************************************************/

//use these nops with the macro OBFUSCATENOPS, or manually insert __asm nop
#define NOP5	__asm nop __asm nop __asm nop __asm nop __asm nop
#define NOP10	NOP5 NOP5
#define NOP20	NOP10 NOP10

#define OBFUSCATENOPS(x)	OBFUSCATENOPS2(x,__LINE__)

//everything between the _START and _ENDE macro will only be executed once, after that
//it is overwritten with random nops.
//use both arguments to create a unique identifier, this is very important!
#define RUNONCE_START(x)	DEL_START3(PASTE(del_start_lbl_,x))
#define RUNONCE_END(x)	DEL_END3(PASTE(del_start_lbl_,x),PASTE(del_end_lbl_,x))



void __stdcall _AddNops(PBYTE start, PBYTE end);
PBYTE __stdcall AllocateJumpgate(PBYTE target, int minlen, int maxlen);
void __stdcall FreeJumpgate(PBYTE target);


struct nop_equ {
	char* opcode;
	int len;
};

#if SIMPLE_OBF_CPP_VERSION != SIMPLE_OBF_H_VERSION
#error Obfuscation header does not match .cpp version
#endif

#if defined(_WIN64) || defined(_M_X64) || defined(_M_IA64)
#error Sorry, 64 bits not supported
#endif

#if defined(_Wp64)
#pragma warning( disable: 4244 4311 )	//conversion from '__w64 int' to 'int' and pointer truncation from 'const PBYTE' to 'int'
#endif

nop_equ possible_opcodes[] = {
	{ "\x90",1 },			//nop
	{ "\xD9\xD0",2 },		//fnop
	{ "\x8B\xC0",2 },		//mov     eax, eax
	{ "\x8B\xDB",2 },		//mov     ebx, ebx
	{ "\x8B\xC9",2 },		//mov     ecx, ecx
	{ "\x8B\xD2",2 },		//mov     edx, edx
	{ "\x8B\xE4",2 },		//mov     esp, esp
	{ "\x8B\xF6",2 },		//mov     esi, esi
	{ "\x8B\xFF",2 },		//mov     edi, edi
	{ "\x87\xDB",2 },		//xchg    ebx, ebx
	{ "\x87\xC9",2 },		//xchg    ecx, ecx
	{ "\x87\xD2",2 },		//xchg    edx, edx
	{ "\x87\xE4",2 },		//xchg    esp, esp
	{ "\x87\xED",2 },		//xchg    ebp, ebp
	{ "\x87\xF6",2 },		//xchg    esi, esi
	{ "\x87\xFF",2 },		//xchg    edi, edi
	{ "\x50\x58",2 },		//push,pop eax
	{ "\x53\x5B",2 },		//push,pop ebx
	{ "\x51\x59",2 },		//push,pop ecx
	{ "\x52\x5A",2 },		//push,pop edx
	{ "\x56\x5E",2 },		//push,pop esi
	{ "\x57\x5F",2 },		//push,pop edi
	{ "\x83\xC8\x00",3 },	//or eax, 0
	{ "\x83\xC9\x00",3 },	//or ecx, 0
	{ "\x83\xCA\x00",3 },	//or edx, 0
	{ "\x83\xCB\x00",3 },	//or ebx, 0
	{ "\x83\xCC\x00",3 },	//or esp, 0
	{ "\x83\xCD\x00",3 },	//or ebp, 0
	{ "\x83\xCE\x00",3 },	//or esi, 0
	{ "\x83\xCF\x00",3 },	//or edi, 0
	{ "\x83\xE0\xFF",3 },	//and eax, FFFFFFFF
	{ "\x83\xE1\xFF",3 },	//and ecx, FFFFFFFF
	{ "\x83\xE2\xFF",3 },	//and edx, FFFFFFFF
	{ "\x83\xE3\xFF",3 },	//and ebx, FFFFFFFF
	{ "\x83\xE4\xFF",3 },	//and esp, FFFFFFFF
	{ "\x83\xE5\xFF",3 },	//and ebp, FFFFFFFF
	{ "\x83\xE6\xFF",3 },	//and esi, FFFFFFFF
	{ "\x83\xE7\xFF",3 },	//and edi, FFFFFFFF
	{ "\x83\xF0\x00",3 },	//xor eax, 0
	{ "\x83\xF1\x00",3 },	//xor ecx, 0
	{ "\x83\xF2\x00",3 },	//xor edx, 0
	{ "\x83\xF3\x00",3 },	//xor ebx, 0
	{ "\x83\xF4\x00",3 },	//xor esp, 0
	{ "\x83\xF5\x00",3 },	//xor ebp, 0
	{ "\x83\xF6\x00",3 },	//xor esi, 0
	{ "\x83\xF7\x00",3 },	//xor edi, 0
	{ 0,0 }
};


int GenRand(int min, int max)
{
	UINT number = 0;
	__asm push edx
	__asm rdtsc
	__asm pop edx
	__asm mov number, eax
	number %= (max - min);
	return number + min;
}

//perform the actual nopping, called from macro's trough _AddNops, or from AllocateJumpgate
void real_AddNops(PBYTE start, PBYTE end)
{
	int iLeft = end - start;
	while (iLeft) {
		if (GenRand(0, 7) < 5) {
			nop_equ* p = possible_opcodes + GenRand(0, ARRAYSIZE(possible_opcodes));
			if (!p->len) continue;
			if (p->len > iLeft) continue;
			for (int i = 0; i < p->len; i++)
				end[-iLeft + i] = p->opcode[i];
			iLeft -= p->len;
		}
		else {
			end[-(iLeft--)] = 0xEB;
			int iSize = GenRand(0, iLeft);
			end[-(iLeft--)] = (BYTE)(iSize & 0xFF);
			while (iSize) {
				end[-iLeft] = (BYTE)(GenRand(0, 256) & 0xFF);
				iSize--;
				iLeft--;
			}
		}
		if (iLeft == 1) {
			end[-1] = 0x90;
			break;
		}
	}
}

//wrapper function, change page protection only when called from macros :)
void __stdcall _AddNops(PBYTE start, PBYTE end)
{
	DWORD dwOld, dwOld2;
	VirtualProtectEx(INVALID_HANDLE_VALUE, start, end - start, PAGE_EXECUTE_READWRITE, &dwOld);
	real_AddNops(start, end);
	VirtualProtectEx(INVALID_HANDLE_VALUE, start, end - start, dwOld, &dwOld2);
}


void WriteJmp(const PBYTE from, const PBYTE to)
{
	*from = 0xE9;
	PDWORD dwJmp = (PDWORD)(from + 1);
	*dwJmp = (int)to - (int)from - 5;
}


PBYTE __stdcall AllocateJumpgate(PBYTE target, int minlen, int maxlen)
{
	PBYTE buf;
	DWORD dwOld;
	int iLen = GenRand(minlen, maxlen);
	buf = (PBYTE)VirtualAllocEx(INVALID_HANDLE_VALUE, NULL, iLen + 5, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	real_AddNops(buf, buf + iLen);
	WriteJmp(buf + iLen, target);
	VirtualProtectEx(INVALID_HANDLE_VALUE, buf, iLen, PAGE_EXECUTE_READ, &dwOld);
	return buf;
}

void __stdcall FreeJumpgate(PBYTE target)
{
	VirtualFreeEx(INVALID_HANDLE_VALUE, target, 0, MEM_RELEASE);
}