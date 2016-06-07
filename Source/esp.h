#pragma once

#include "client-entity-list.h"
#include "engine-client.h"
#include "entity.h"

#include "render.h"

class CESP
{
public:
	void DrawGlow(DWORD GlowPtr, int entity, float r, float g, float b, float a) {
		*(float*)((GlowPtr + ((entity * 0x38) + 0x4))) = r;
		*(float*)((GlowPtr + ((entity * 0x38) + 0x8))) = g;
		*(float*)((GlowPtr + ((entity * 0x38) + 0xC))) = b;
		*(float*)((GlowPtr + ((entity * 0x38) + 0x10))) = a;
		*(bool*)((GlowPtr + ((entity * 0x38) + 0x24))) = true;
		*(bool*)((GlowPtr + ((entity * 0x38) + 0x25))) = false;
		*(bool*)((GlowPtr + ((entity * 0x38) + 0x26))) = false;
	}
	void Think(CBaseEntity* Local)
	{

		PlayerInfo_t Info;

		Matrix3x4_t Matrix = EngineClient->GetMatrix( );

		CVector Screen, Head;
		int spectators = 0;
		for ( int i = 0; i < maxEntities; ++i )
		{
			
			CBaseEntity* Entity = ( CBaseEntity* ) ClientEntityList->GetClientEntity( i );		

			if ( !Entity )
				continue;
			
			if ( Entity == Local )
				continue;

			if ( Entity->GetDormant( ) )
				continue;
			//if (isSpectator(Entity, Local))
			//	spectators++;
			//	continue;
			CColor Color(0, 0, 0, 0);
			if (Entity->GetTeam() == Local->GetTeam())
				Color = CColor(0, 255, 0, 255);
			else
				Color = CColor(255, 0, 0, 255);
			//Render->DrawF(10, 200, CColor(255, 255, 255, 255), 5, false, "Number of your specatators: %d", spectators);
		
			if ( Entity->GetHealth( ) > 0)
			{
				
				EngineClient->GetPlayerInfo( i, &Info );
				CVector vTop, vBot;
				vBot = Entity->GetOrigin();
				if (Entity->GetFlags() & FL_DUCKING)
					vTop = vBot + CVector(0, 0, 50.f);
				else
					vTop = vBot + CVector(0, 0, 70.f);

				if ( WorldToScreen( vBot, Screen, Matrix ) && WorldToScreen( vTop, Head, Matrix ) )
				{
					//DrawGlow(GlowPointer, i, 1.f, 0.f, 0.f, 1.f);
					
					
					float flashDuration = Entity->GetFlashDuration();
					if (flashDuration > 5.f) {
						Color = CColor(128, 0, 128, 255);
					}
					Render->DrawF( Head.x, Head.y - 30, Color, 5, 1, "%s", Info.szName);
					Render->DrawF( Head.x, Head.y - 15, Color, 5, 1, "- %d -", Entity->GetHealth( ) );
					float Factor = Entity->GetHealth() / 100;
					int Height = Screen.y - Head.y, Width = Height / 2;
					int Health = Entity->GetHealth();
					int Red = 255 - (Health*2.55), Green = Health * 2.55;
					CColor Healthbar = CColor(Red, Green, 0, 255);
					CVector AimPos = Entity->GetBonePosition(6);
					CVector BonesPos;
					int iWidth = Width / 6;
					
					

					Render->DrawEsp( Head.x - Width / 1.75, Head.y, Width, Height, Color );
					if (Local->GetTeam() != Entity->GetTeam()) {
						if (WorldToScreen(AimPos, BonesPos, Matrix)) {
							Render->DrawInlineRect(BonesPos.x - iWidth, BonesPos.y - iWidth, iWidth * 2, iWidth * 2, CColor(255, 0, 0, 255));
						}
						Render->FillRGB(Head.x - Width / 1.3, Head.y, Health * Height / 100, Width / 15, Healthbar);
					}
						
				}
				

			}


		}

	}

	__forceinline bool WorldToScreen(CVector In, CVector& Out, Matrix3x4_t ViewMatrix) // fix; move matrix to this instead of passing? might look nicer.
	{
		Out.x = ViewMatrix.Matrix[0] * In.x + ViewMatrix.Matrix[1] * In.y + ViewMatrix.Matrix[2] * In.z + ViewMatrix.Matrix[3];

		Out.y = ViewMatrix.Matrix[4] * In.x + ViewMatrix.Matrix[5] * In.y + ViewMatrix.Matrix[6] * In.z + ViewMatrix.Matrix[7];

		Out.z = ViewMatrix.Matrix[12] * In.x + ViewMatrix.Matrix[13] * In.y + ViewMatrix.Matrix[14] * In.z + ViewMatrix.Matrix[15];

		if (Out.z < 0.01f)
			return false;

		float Inverse = 1.f / Out.z;

		Out.x *= Inverse;
		Out.y *= Inverse;

		int Width, Height;

		EngineClient->GetScreenSize(Width, Height);

		auto X = Width / 2;
		auto Y = Height / 2;

		X += 0.5 * Out.x * Width + 0.5;
		Y -= 0.5 * Out.y * Height + 0.5;

		Out.x = X;
		Out.y = Y;

		return true;
	}
	bool isSpectator(CBaseEntity* Entity, CBaseEntity* Local) {
		if (!(Entity->GetTeam() == 2)) return false;
		if (!(Entity->GetEyePosition() == Local->GetEyePosition())) return false;
		return true;
	}

};
