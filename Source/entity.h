#pragma once

#include "top.h"
#include "vector.h"

#define	FL_ONGROUND				(1<<0)// At rest / on the ground
#define FL_DUCKING				(1<<1)// Player flag -- Player is fully crouched
#define	FL_WATERJUMP			(1<<2)// player jumping out of water
#define FL_ONTRAIN				(1<<3)// Player is _controlling_ a train, so movement commands should be ignored on client during prediction.
#define FL_INRAIN				(1<<4)// Indicates the entity is standing in rain
#define FL_FROZEN				(1<<5)// Player is frozen for 3rd person camera
#define FL_ATCONTROLS			(1<<6)// Player can't move, but keeps key inputs for controlling another entity
#define	FL_CLIENT				(1<<7)// Is a player
#define FL_FAKECLIENT			(1<<8)// Fake client, simulated server side; don't send network messages to them
#define	FL_INWATER				(1<<9)// In water
struct bone_t
{
	char _pad01[12];
	float x;

	char _pad02[12];
	float y;

	char _pad03[12];
	float z;
};
class CBaseEntity
{
public:

	bool GetDormant( )
	{
		return *( bool* ) ( this + 0xE9 );
	}
	bool GetLifeState() {
		return *( bool* )(this + 0x25B);
	}
	int GetTeam( )
	{
		return *( int* ) ( this + 0xF0 );
	}

	int GetHealth( )
	{
		return *( int* ) ( this + 0xFC );
	}

	int GetFlags( )
	{
		return *( int* ) ( this + 0x100 );
	}

	float GetFlashDuration()
	{
		return *(float*)(this + 0xA2F8);
	}

	CVector GetVelocity()
	{
		return *(CVector *)(this + 0x110);
	}

	CVector GetPunch() 
	{
		return *( CVector *) ( this + 0x3018);
	}

	CVector GetOrigin( )
	{
		return *( CVector* ) ( this + 0x134 );
	}

	CVector GetEyePosition( )
	{
		return *( CVector* ) ( this + 0x104 ) + GetOrigin( );
	}

	CVector GetBonePosition(int bone) {
		CVector vec;
		bone_t bones;
		DWORD address = *(DWORD*)(this + 0x2698); //bonematrix
		bones = *(bone_t*)(address + (0x30 * bone));
		vec.x = bones.x;
		vec.y = bones.y;
		vec.z = bones.z;
		return vec;
	}
	char* GetLastPlace() {
		return ((char*) this + 0x3594); //m_szLastPlaceName
	}
};