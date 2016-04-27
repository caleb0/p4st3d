#pragma once

#include "top.h"
#include "matrix3x4.h"
#include "playerinfo.h"

class IVEngineClient
{
public:
	void GetScreenSize( int& Width, int& Height )
	{
		typedef void( __thiscall* _GetScreenSize )( void*, int&, int& );
		return EmulateVirtual< _GetScreenSize >( this, 5 )( this, Width, Height );
	}
	void GetPlayerInfo( int Index, PlayerInfo_t* _Info )
	{
	
		typedef void( __thiscall* _GetPlayerInfo )( void*, int, PlayerInfo_t* );
		return EmulateVirtual<_GetPlayerInfo>( this, 8 )( this, Index, _Info );
	}

	int GetLocalPlayer( )
	{
	
		typedef int( __thiscall* _GetLocalPlayer )( void* );
		return EmulateVirtual<_GetLocalPlayer>( this, 12 )( this );
	}
	void SetViewAngles(CVector& Angles)
	{
		typedef void(__thiscall* _SetViewAngles)(void*, CVector&);
		return EmulateVirtual<_SetViewAngles>(this, 19)(this, Angles);
	}
	bool IsInGame( )
	{
		
		typedef bool( __thiscall* _IsInGame )( void* );
		return EmulateVirtual<_IsInGame>( this, 26 )( this );
	}

	bool IsConnected( )
	{
		
		typedef bool( __thiscall* _IsConnected )( void* );
		return EmulateVirtual<_IsConnected>( this, 27 )( this );
	}

	int GetMaxClients( )
	{
		
		typedef int( __thiscall* _GetMaxClients )( void* );
		return EmulateVirtual<_GetMaxClients>( this, 20 )( this );
	}

	const Matrix3x4_t& GetMatrix( )
	{
		typedef const Matrix3x4_t& ( __thiscall* _GetMatrix )( void* );
		return EmulateVirtual< _GetMatrix>( this, 37 )( this );
	}
	
	void ClientCmd(const char* szCmdString)
	{
		typedef void(__thiscall* _ClientCmd)(void*, const char* );
		return EmulateVirtual< _ClientCmd>(this, 108)(this, szCmdString);
	}
	void CliendCmd_Unrestricted(const char* szCmdString) {
		typedef void(__thiscall* OriginalFn)(void*, const char*);
		return EmulateVirtual<OriginalFn>(this, 114)(this, szCmdString);
	}
}; extern IVEngineClient * EngineClient;
