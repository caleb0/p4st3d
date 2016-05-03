#pragma once

#pragma region includes

#include <Windows.h>
#include <iostream>
#include <fstream>
#include <Psapi.h>
#include "usercmd.h"

#pragma endregion

#pragma region protos

typedef void*	( *Interface )( char* _Name, int Return );

typedef void( __thiscall *PaintTraverse )( void*, unsigned int, bool, bool );

typedef bool( __thiscall *CreateMove )( void*, float, CUserCmd* );

typedef void(__cdecl* MsgFn)(const char* msg, va_list);
#pragma endregion

class CTools
{
public:

	virtual void* GetInterface( char* _Module, char* _Interface )
	{
		Interface TempInterface = ( Interface ) GetProcAddress( GetModuleHandle( _Module ), "CreateInterface" );

		return ( void* ) TempInterface( _Interface, 0 );
	}

	bool bCompare(const BYTE* Data, const BYTE* Mask, const char* szMask)
	{
		for (; *szMask; ++szMask, ++Mask, ++Data)
		{
			if (*szMask == 'x' && *Mask != *Data)
			{
				return false;
			}
		}
		return (*szMask) == 0;
	}
	DWORD WaitOnModuleHandle(std::string moduleName)
	{
		DWORD ModuleHandle = NULL;
		while (!ModuleHandle)
		{
			ModuleHandle = (DWORD)GetModuleHandle(moduleName.c_str());
			if (!ModuleHandle)
				Sleep(50);
		}
		return ModuleHandle;
	}

	DWORD FindPattern(std::string moduleName, BYTE* Mask, char* szMask)
	{
		DWORD Address = WaitOnModuleHandle(moduleName.c_str());
		MODULEINFO ModInfo; GetModuleInformation(GetCurrentProcess(), (HMODULE)Address, &ModInfo, sizeof(MODULEINFO));
		DWORD Length = ModInfo.SizeOfImage;
		for (DWORD c = 0; c < Length; c += 1)
		{
			if (bCompare((BYTE*)(Address + c), Mask, szMask))
			{
				return (DWORD)(Address + c);
			}
		}
		return 0;
	}

	DWORD FindTextPattern(std::string moduleName, char* string)
	{
		DWORD Address = WaitOnModuleHandle(moduleName.c_str());
		MODULEINFO ModInfo; GetModuleInformation(GetCurrentProcess(), (HMODULE)Address, &ModInfo, sizeof(MODULEINFO));
		DWORD Length = ModInfo.SizeOfImage;

		int len = strlen(string);
		char* szMask = new char[len + 1];
		for (int i = 0; i < len; i++)
		{
			szMask[i] = 'x';
		}
		szMask[len] = '\0';

		for (DWORD c = 0; c < Length; c += 1)
		{
			if (bCompare((BYTE*)(Address + c), (BYTE*)string, szMask))
			{
				return (DWORD)(Address + c);
			}
		}
		return 0;
	}
}; extern CTools * Tools;


void Msg(const char* msg, ...)
{
	if (msg == nullptr)
		return; //If no string was passed, or it was null then don't do anything
	static MsgFn fn = (MsgFn)GetProcAddress(GetModuleHandle("tier0.dll"), "Msg"); //This gets the address of export "Msg" in the dll "tier0.dll". The static keyword means it's only called once and then isn't called again (but the variable is still there)
	char buffer[989];
	va_list list; //Normal varargs stuff http://stackoverflow.com/questions/10482960/varargs-to-printf-all-arguments
	va_start(list, msg);
	vsprintf_s(buffer, msg, list);
	va_end(list);
	fn(buffer, list); //Calls the function, we got the address above.
}

template< typename Function > Function EmulateVirtual( void* _VMT, int Index )
{
	void*** _TVMT = ( void*** ) _VMT;

	void** VMT = *_TVMT;

	void* _Address = VMT[ Index ];

	return ( Function ) ( _Address );
}