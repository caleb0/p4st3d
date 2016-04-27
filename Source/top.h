#pragma once

#pragma region includes

#include <Windows.h>
#include <iostream>

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

	DWORD FindPattern(char * pattern, char* mask, DWORD start, DWORD end, DWORD offset) {
		int patternLength = strlen(mask);
		bool found = false;

		for (DWORD i = start; i < end - patternLength; i++) {
			found = true;
			for (int idx = 0; idx < patternLength; idx++) {
				if (mask[idx] == 'x' && pattern[idx] != *(char*)(i + idx)) {
					found = false;
					break;
				}
			}
			if (found) {
				return i + offset;
			}
		}
		return NULL;
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