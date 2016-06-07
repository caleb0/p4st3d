
void AntiAim(CUserCmd* pCmd) {
	if (pCmd->Buttons & IN_ATTACK || pCmd->Buttons & IN_ATTACK2 || pCmd->Buttons & IN_ATTACK3) return;
	pCmd->ViewAngles.y -= 180;
}
void SlowSpin(CUserCmd *pCmd)
{
	pCmd->ViewAngles.x = -88;
	int random = rand() % 10;

	static bool dir;
	static float change;

	change += 10;
	change += random;

	pCmd->ViewAngles.y += change;

}
void FastSpint(CUserCmd *pCmd)
{
	float goldern = 360 / 1.61803398875;
	static float current_y = pCmd->ViewAngles.y + goldern;
	pCmd->ViewAngles.y = current_y;
}
void Pitch(CUserCmd* pCmd) {
	pCmd->ViewAngles.x = -88;
}
int iSpin = 0;

void SilentXSpin(CUserCmd* cmd, int& iSpinx) {
	iSpinx++;

	if (iSpinx > 1) {
		iSpinx = 0;
	}

	if (iSpinx == 1) {
		cmd->ViewAngles.x = 89;
	}
}
void CorrectLisp(CUserCmd* pCmd) {
	if (GetAsyncKeyState('W'))
		pCmd->ForwardMove = 450.f;
	if (GetAsyncKeyState('S'))
		pCmd->ForwardMove = -450.f;
}

CVector viewforward, viewright, viewup, aimforward, aimright, aimup, vForwardNorm, vRightNorm, vUpNorm;
float forward, right, up;

void FixStart(CUserCmd* pCmd)
{
	CVector angle = pCmd->ViewAngles;
	angle.x = 0;
	forward = pCmd->ForwardMove;
	right = pCmd->SideMove;
	up = pCmd->UpMove;
	AngleVectors(angle, (float*)&viewforward, (float*)&viewright, (float*)&viewup);
}

float DotProduct(CVector v1, CVector v2)
{
	return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
}
void Flip(CUserCmd *pCmd)
{
	static bool back = false;
	back = !back;
	if (back)
		pCmd->ViewAngles.y -= 180;
}
void Normalize(CVector &vecIn, CVector &vecOut)
{
	float flLen = vecIn.Length();

	if (flLen == 0) {
		vecOut.Init(0, 0, 1);
		return;
	}

	flLen = 1 / flLen;
	vecOut.Init(vecIn.x * flLen, vecIn.y * flLen, vecIn.z * flLen);
}
void FixEnd(CUserCmd* pCmd)
{
	CVector angle = pCmd->ViewAngles;
	angle.x = 0;
	AngleVectors(angle, (float*)&aimforward, (float*)&aimright, (float*)&aimup);
	Normalize(viewforward, vForwardNorm);
	Normalize(viewright, vRightNorm);
	Normalize(viewup, vUpNorm);
	pCmd->ForwardMove = DotProduct(vForwardNorm * forward, aimforward) + DotProduct(vRightNorm * right, aimforward) + DotProduct(vUpNorm * up, aimforward);
	pCmd->SideMove = DotProduct(vForwardNorm * forward, aimright) + DotProduct(vRightNorm * right, aimright) + DotProduct(vUpNorm * up, aimright);
}

void ReverseMovement(CUserCmd* pCmd) {
	double fMove = pCmd->ForwardMove;
	double sMove = pCmd->SideMove;
	pCmd->ForwardMove = fMove * -1;
	pCmd->SideMove = sMove * -1;
}
void fakedown(CUserCmd* pCmd) {
	pCmd->ViewAngles.x = 89.00f;
}
void DoAntiAim(CUserCmd* pCmd) {
	CVector vMove(pCmd->ForwardMove, pCmd->SideMove, pCmd->UpMove);
	float flSpeed = sqrt(vMove.x * vMove.x + vMove.y * vMove.y), flYaw;
	CVector vMove2;
	CVector vRealView(pCmd->ViewAngles);
	VectorAngles3D(vMove, vMove2);

	//SlowSpin(pCmd);
	FastSpint(pCmd);
	//fix movement
	flYaw = DEG2RAD(pCmd->ViewAngles.y - vRealView.y + vMove2.y);
	pCmd->ForwardMove = cos(flYaw) * flSpeed;
	pCmd->SideMove = sin(flYaw) * flSpeed;
}
