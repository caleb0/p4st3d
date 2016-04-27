bool bSmooth = true;
bool silent = true;
class CAim {

public: 
	void Think(CBaseEntity* Local) {
		if (!GetAsyncKeyState(VK_XBUTTON2)) return;
		if (Local->GetHealth() <= 0) return;
		static Angle angAimAngles;
		static int iClosestIndex;
		iClosestIndex = ClosestAngle(angAimAngles, Local);
		if (iClosestIndex != -1) {
			angAimAngles.x -= (Local->GetPunch().x * 2.f);
			angAimAngles.y -= (Local->GetPunch().y * 2.f);


			GUserCmd->ViewAngles = angAimAngles;
			GUserCmd->ViewAngles.Normalize();
			ClampAngles(GUserCmd->ViewAngles);

			GUserCmd->ForwardMove = 0;
			GUserCmd->SideMove = 0;
			GUserCmd->UpMove = 0;

			GUserCmd->Buttons |= IN_DUCK;
			GUserCmd->Buttons |= IN_ATTACK;
		}

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
	int ClosestAngle(CVector& angDestination, CBaseEntity* Local) {
		static CVector vecLocalPos;
		static CVector vecEntityPos;
		int iClosestIndex = -1;
		Angle angAimAngles[64];
		for (int i = 0; i < 64; i++) {
			CBaseEntity* Entity = (CBaseEntity*)ClientEntityList->GetClientEntity(i);
			if (!IsValidTarget(Entity, Local)) continue;
			vecLocalPos = Local->GetEyePosition();
			vecEntityPos = Entity->GetBonePosition(6);
			CalcAngle(vecLocalPos, vecEntityPos, angAimAngles[i]);
			angAimAngles[i].Normalize();
			float flDifference = GUserCmd->ViewAngles.Difference(angAimAngles[i]);
			if (flDifference < 360.f) {
				iClosestIndex = i;
			}

		}
		angDestination = angAimAngles[iClosestIndex];
		return iClosestIndex;
	}

	bool IsValidTarget(CBaseEntity* Entity, CBaseEntity* Local) {
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
		return (Trace.m_pEnt != NULL && Trace.m_pEnt == Entity) || Trace.fraction == 1.f;
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

	bool AimStep(Angle angSource, Angle angDestination, Angle &angOut)
	{
		angDestination.Normalize();
		Angle angDelta = (angDestination - angSource).Normalize();;
		int iStepAmount = 15.f;
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

		GUserCmd->ViewAngles.Normalize();

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

		/*angDestination.Normalize();
		Angle angDelta = (angDestination - angSource).Normalize();;
		static float stepamount = 10.f;
		bool bXFinished = true;
		bool bYFinished = true;

		angSource.x += Util::Clamp(angDelta.x, -stepamount, stepamount);
		angSource.y += Util::Clamp(angDelta.y, -stepamount, stepamount);

		if ((angDelta.x < -stepamount) || (angDelta.x > stepamount))
		bXFinished = false;

		if ((angDelta.y < -stepamount) || (angDelta.y > stepamount))
		bXFinished = false;

		angSource.Normalize();

		angOut = angSource;

		return bXFinished && bYFinished;*/
	}
};

class CTrigger {
public: 
	void Think(CBaseEntity* Local) {
		if (!GetAsyncKeyState(VK_XBUTTON1)) return;
		if (Local->GetHealth() <= 0) return;
		for (int i = 0; i < 64; i++) {
			CBaseEntity* currentEntity = (CBaseEntity*)ClientEntityList->GetClientEntity(i);
			if (isValidTarget(currentEntity, Local)) {
				GUserCmd->Buttons |= IN_ATTACK;
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