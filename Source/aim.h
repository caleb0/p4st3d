
class CAim {
	bool shouldShoot = false;
public: 
	void Think(CUserCmd* pCmd, CBaseEntity* Local) {
		if (Local->GetHealth() <= 0) return;
		static Angle angAimAngles;
		static int iClosestIndex;
		iClosestIndex = ClosestAngle(pCmd,angAimAngles, Local);
		
		if (iClosestIndex != -1) {
			angAimAngles.x -= (Local->GetPunch().x * 2.f);
			angAimAngles.y -= (Local->GetPunch().y * 2.f);
			//bool shouldShoot = Aimstep(pCmd->ViewAngles, angAimAngles, angAimAngles, 3);
			angAimAngles.Normalize();
			ClampAngles(angAimAngles);
			pCmd->ViewAngles = angAimAngles;
			pCmd->ForwardMove = 0;
			pCmd->SideMove = 0;
			pCmd->UpMove = 0;
			pCmd->Buttons |= IN_DUCK;
			pCmd->Buttons |= IN_ATTACK;
			
			
			
		}

	}
	bool AimStep(Angle angSource, Angle angDestination, Angle &angOut)
	{
		angDestination.Normalize();
		Angle angDelta = (angDestination - angSource).Normalize();
		int iStepAmount = 1.f; // above 15 gets me kicked.
		bool bXFinished = false;
		bool bYFinished = false;

		if (angDelta.x > iStepAmount)
			angSource.x += iStepAmount;
		else if (angDelta.x < -iStepAmount)
			angSource.x -= iStepAmount;
		else
		{
			bXFinished = true;
			angSource.x = angDestination.x;
		}

		if (angDelta.y > iStepAmount)
			angSource.y += iStepAmount;
		else if (angDelta.y < -iStepAmount)
			angSource.y -= iStepAmount;
		else
		{
			bYFinished = true;
			angSource.y = angDestination.y;
		}

		angSource.Normalize();

		angOut = angSource;

		return bXFinished && bYFinished;
	}
	bool Aimstep(Angle src, Angle dst, Angle& out, int steps)
	{
		Angle delta_angle = (dst - src).Normalize();;
		bool x_finished = false;
		bool y_finished = false;

		if (delta_angle.x > steps)
			src.x += steps;

		else if (delta_angle.x < -steps)
			src.x -= steps;

		else
		{
			x_finished = true;
			src.x = dst.x;
		}

		if (delta_angle.y > steps)
			src.y += steps;

		else if (delta_angle.y < -steps)
			src.y -= steps;

		else
		{
			y_finished = true;
			src.y = dst.y;
		}

		src.Normalize();

		out = src;

		return x_finished && y_finished;
	}


	Angle Smooth(Angle pOriginal, Angle angDestination, float fSmoothness)
	{
		Angle angSmooth;
		angSmooth.Init();

		angSmooth.x = angDestination.x - pOriginal.x;
		angSmooth.y = angDestination.y - pOriginal.y;

		angSmooth.Normalize();

		angSmooth.x = pOriginal.x + angSmooth.x / 100 * fSmoothness;
		angSmooth.y = pOriginal.y + angSmooth.y / 100 * fSmoothness;

		angSmooth.Normalize();

		return angSmooth;
	}
	int ClosestAngle(CUserCmd* pCmd, CVector& angDestination, CBaseEntity* Local) {
		static CVector vecLocalPos;
		static CVector vecEntityPos;
		int iClosestIndex = -1;
		Angle angAimAngles[64];
		for (int i = 0; i < maxEntities; i++) {
			CBaseEntity* Entity = (CBaseEntity*)ClientEntityList->GetClientEntity(i);
			if (!IsValidTarget(Entity, Local)) continue;
			vecLocalPos = Local->GetEyePosition();
			vecEntityPos = Entity->GetBonePosition(6);
			CalcAngle(vecLocalPos, vecEntityPos, angAimAngles[i]);

			angAimAngles[i].Normalize();
			float flDifference = pCmd->ViewAngles.Difference(angAimAngles[i]);
			
			if (flDifference < 360.f) {
				iClosestIndex = i;
			}
			
		}
		angDestination = angAimAngles[iClosestIndex];
		return iClosestIndex;
	}

	int SoftClosestAngle(CUserCmd* pCmd, CBaseEntity* Local) {
		static CVector vecLocalPos;
		static CVector vecEntityPos;
		int iClosestIndex = -1;
		Angle angAimAngles[64];
		for (int i = 0; i < maxEntities; i++) {
			CBaseEntity* Entity = (CBaseEntity*)ClientEntityList->GetClientEntity(i);
			if (!IsValidTarget(Entity, Local)) continue;
			vecLocalPos = Local->GetEyePosition();
			vecEntityPos = Entity->GetBonePosition(6);
			CalcAngle(vecLocalPos, vecEntityPos, angAimAngles[i]);
			
			angAimAngles[i].Normalize();
			float flDifference = pCmd->ViewAngles.Difference(angAimAngles[i]);
			if (flDifference < 180.f) {
				iClosestIndex = i;
			}
		}
		return iClosestIndex;
	}
	
	bool IsValidTarget(CBaseEntity* Entity, CBaseEntity* Local) {
		if (!Entity) return false;
		if (Entity == Local) return false;
		if (!Entity->GetLifeState() == 0) return false;
		if (Entity->GetHealth() <= 0 || Entity->GetHealth() > 100) return false;
		if (Entity->GetDormant()) return false;
		if (Entity->GetTeam() == Local->GetTeam()) return false;
		if (!TraceRay(Local, Entity->GetBonePosition(6))) return false;

		return true;
	}
	bool TraceRay(CBaseEntity* Local, CVector aimpos) {
		CVector eyepos = Local->GetEyePosition();
		trace_t Trace;
		Ray_t Ray;
		CTraceFilter Filter;
		Ray.Init(eyepos, aimpos);
		Filter.pSkip1 = Local;
		EngineTrace->TraceRay(Ray, 0x4600400B, &Filter, &Trace);
		return Trace.fraction == 1.f;
	}
	void CalcAngle(CVector& vecSource, CVector& vecDestination, Angle& angAngles)
	{
		CVector vDelta = vecSource - vecDestination;
		float hyp = sqrt(vDelta.x * vDelta.x + vDelta.y * vDelta.y);
		angAngles.x = atan(vDelta.z / hyp) * 57.295779513082f;
		angAngles.y = atan(vDelta.y / vDelta.x) * 57.295779513082f;
		angAngles.z = 0.f;
		if (vDelta.x >= 0.f) angAngles.y += 180.f;
	}

	
};

class CTrigger {
public: 
	void Think(CBaseEntity* Local, CUserCmd* pCmd) {
		if (!GetAsyncKeyState(VK_XBUTTON1)) return;
		if (Local->GetHealth() <= 0) return;
		for (int i = 0; i < maxEntities; i++) {
			CBaseEntity* currentEntity = (CBaseEntity*)ClientEntityList->GetClientEntity(i);
			if (isValidTarget(currentEntity, Local)) {
				pCmd->Buttons |= IN_ATTACK;
			}
		}

	}
	bool isValidTarget(CBaseEntity* Entity, CBaseEntity* Local) {
			if (!Entity) return false;
			if (Entity == Local) return false;
			if (!Entity->GetLifeState() == 0) return false;
			if (Entity->GetHealth() <= 0 || Entity->GetHealth() > 100) return false;
			if (Entity->GetDormant()) return false;
			if (Entity->GetTeam() == Local->GetTeam()) return false;
			if (!TraceRay(Local, Entity)) return false;

			return true;
	
	}
	bool TraceRay(CBaseEntity* Local, CBaseEntity* Entity) {
		CVector eyepos = Local->GetEyePosition();
		CVector aimpos = Entity->GetBonePosition(6);
		trace_t Trace;
		Ray_t Ray;
		CTraceFilter Filter;
		Ray.Init(eyepos, aimpos);
		Filter.pSkip1 = Local;
		EngineTrace->TraceRay(Ray, 0x4600400B, &Filter, &Trace);
		return (Trace.m_pEnt != NULL && Trace.m_pEnt == Entity) && Trace.fraction == 1.f;
	}
};