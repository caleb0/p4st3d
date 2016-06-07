
#include "main.h"
#include <sapi.h> 
#include <sphelper.h>


std::unique_ptr<VFTableHook>gPanelHook = nullptr;
std::unique_ptr<VFTableHook>gCreateMoveHook = nullptr;
std::unique_ptr<VFTableHook>gOverrideMouseHook = nullptr;
std::unique_ptr<VFTableHook>gDrawModelHook = nullptr;


char* bSendPacket;
static ConVar* name;

static float rainbow;

void ErasePE()
{
	char *pBaseAddr = (char*)GetModuleHandle(NULL);
	DWORD dwOldProtect = 0;
	VirtualProtect(pBaseAddr, 4096, PAGE_READWRITE, &dwOldProtect);
	ZeroMemory(pBaseAddr, 4096);
	VirtualProtect(pBaseAddr, 4096, dwOldProtect, &dwOldProtect);
}
void getRecoil(CVector Punch) {
	x = ScreenWidth / 2;
	y = ScreenHeight / 2;
	dHeight = ScreenHeight / 90;
	dWidth = ScreenWidth / 90;
	x -= (dWidth*(Punch.y));
	y += (dHeight*(Punch.x));
}

bool once = true;
void __stdcall HookedPaintTraverse(int VGUIPanel, bool ForceRepaint, bool AllowForce)
{
	_PaintTraverse(Panel, VGUIPanel, ForceRepaint, AllowForce);
	if (!strcmp("FocusOverlayPanel", Panel->GetName(VGUIPanel)))
	{
		if (EngineClient->IsInGame())
		{
			ESP->Think(LocalPlayer);
			Render->DrawRect(x - 3, y - 3, 6, 6, CColor(255, 255, 255, 255));
		}

	}
}
int counter = 0;
bool bName = false;
/* cmove hook function */
bool __stdcall HookedCreateMove(float SampleTime, CUserCmd* UserCmd)
{

	GUserCmd = UserCmd;
	if (!UserCmd->CommandNumber)
		return true;

	LocalPlayer = (CBaseEntity*)ClientEntityList->GetClientEntity(EngineClient->GetLocalPlayer());
	if (!LocalPlayer)
		return true;
	static bool bDirection = false;

	if (UserCmd->Buttons & IN_JUMP && !(LocalPlayer->GetFlags() & FL_ONGROUND))
	{
		UserCmd->Buttons &= ~IN_JUMP;
	}
	/*if ((GetAsyncKeyState(VK_SPACE) && !(LocalPlayer->GetFlags() & FL_ONGROUND))) {
		if (bDirection)
		{
			UserCmd->ViewAngles.y -= 10;
			UserCmd->SideMove = -450.f; // Leak

			bDirection = false;
		}
		else
		{
			UserCmd->ViewAngles.y += 10;
			UserCmd->SideMove = 450.f;

			bDirection = true;
		}
	}*/



	if (GetAsyncKeyState(VK_XBUTTON2)) {
		Aim->Think(UserCmd, LocalPlayer);
	}
	else if (GetAsyncKeyState(VK_XBUTTON1)) {
		DoAntiAim(UserCmd);
	}

	maxEntities = EngineClient->GetMaxClients();

	getRecoil(LocalPlayer->GetPunch());

	UserCmd->ViewAngles.Normalize();
	ClampAngles(UserCmd->ViewAngles);
	return false;
}

IMaterial* hidden_tex;
void __stdcall hkDrawModelExecute(void* context, void* state, const ModelRenderInfo_t& info, Matrix3x4_t* pCustomBoneToWorld) {
	
	if (info.pModel) {
		/*std::string ModelName = ModelInfo->GetModelName((model_t*)info.pModel);
		if (ModelName.find("flash") != std::string::npos) {
			IMaterial* flash = MaterialSystem->FindMaterial("effects\\flashbang", TEXTURE_GROUP_CLIENT_EFFECTS);
			IMaterial* flashWhite = MaterialSystem->FindMaterial("effects\\flashbang_white", TEXTURE_GROUP_CLIENT_EFFECTS);
			flash->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
			flashWhite->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);
			ModelRender->ForcedMaterialOverride(flash);
			ModelRender->ForcedMaterialOverride(flashWhite);
		}*/

	}
	
	_DrawModelExecute(context, state, info, pCustomBoneToWorld);
	ModelRender->ForcedMaterialOverride(NULL);
}

void __stdcall hkOverrideMouseInput(float *_x, float *_y) {

	_OverrideMouseInput(ClientMode, _x, _y);

	if (*(float*)_x == 0.0f || *(float*)_y == 0.0f)
		return;
	LocalPlayer = (CBaseEntity*)ClientEntityList->GetClientEntity(EngineClient->GetLocalPlayer());
	if (!LocalPlayer)
		return;
	if (!(LocalPlayer->GetHealth() > 0))
		return;
	Angle ViewAngles;
	EngineClient->GetViewAngles(ViewAngles);
	ViewAngles.x -= (LocalPlayer->GetPunch().x * 2.f);
	ViewAngles.y -= (LocalPlayer->GetPunch().y * 2.f);
	ViewAngles.Normalize();
	CVector vecPlayer = LocalPlayer->GetOrigin();
	CVector vecPlayerEyes = vecPlayer + LocalPlayer->GetEyePosition();

	int target = Aim->SoftClosestAngle(GUserCmd, LocalPlayer);

	if (target != -1) {
		CBaseEntity* pEntity = (CBaseEntity*)ClientEntityList->GetClientEntity(target);
		if (!pEntity) return;
		CVector bonePos = pEntity->GetBonePosition(6);
		Angle EntityAngles;
		Aim->CalcAngle(vecPlayerEyes, bonePos, EntityAngles);
		EntityAngles.Normalize();
		ClampAngles(EntityAngles);

		Angle Delta = ViewAngles - EntityAngles;
		Delta.Normalize();
		ClampAngles(Delta);

		Delta.x /= 0.022f;
		Delta.y /= 0.022f;

		CVector vDelta = CVector(Delta.y, -Delta.x, 0.0f);

		*(float*)_x = vDelta.x;
		*(float*)_y = vDelta.y;
		return;
	}

}
/* main */

void __stdcall Start( )
{
	
	Tools = new CTools;
	Panel = ( IPanel* )Tools->GetInterface( "vgui2.dll", "VGUI_Panel009" );
	Surface	= ( ISurface* )Tools->GetInterface( "vguimatsurface.dll", "VGUI_Surface031" );
	EngineClient = ( IVEngineClient* )Tools->GetInterface( "engine.dll", "VEngineClient013" );
	ClientEntityList = ( IClientEntityList* )Tools->GetInterface( "client.dll",	"VClientEntityList003" );
	BaseClientDll = ( IBaseClientDll* ) Tools->GetInterface( "client.dll", "VClient017" );
	pCvar = (ICVar*)Tools->GetInterface("vstdlib.dll", "VEngineCvar007");
	EngineTrace = (IEngineTrace*)Tools->GetInterface("engine.dll", "EngineTraceClient004");

	RenderView = (CVRenderView*)Tools->GetInterface("engine.dll", "VEngineRenderView014");
	ModelRender = (IVModelRender*)Tools->GetInterface("engine.dll", "VEngineModel016");
	MaterialSystem = (CMaterialSystem*)Tools->GetInterface("materialsystem.dll", "VMaterialSystem080");
	ModelInfo = (CModelInfo*)Tools->GetInterface("engine.dll", "VModelInfoClient004");
	
	void** BaseClientDllVMT = *( void*** ) BaseClientDll;
	do {
		ClientMode = *(IClientMode***)((DWORD)BaseClientDllVMT[10] + 5);
	} while (ClientMode == NULL && !ClientMode);
	

	ESP = new CESP;
	gCreateMoveHook = std::make_unique<VFTableHook>(*(DWORD***)ClientMode, true);
	gPanelHook = std::make_unique<VFTableHook>((DWORD**)Panel, true);
	//gDrawModelHook = std::make_unique<VFTableHook>((DWORD**)ModelRender, true);
	

	_CreateMove = gCreateMoveHook->Hook(24, (fnCreateMove)HookedCreateMove);
	_PaintTraverse = gPanelHook->Hook(41, (fnPaintTraverse)HookedPaintTraverse);
	//_DrawModelExecute = gDrawModelHook->Hook(21, (fnDrawModelExecute)hkDrawModelExecute);
	

	//gOverrideMouseHook = std::make_unique<VFTableHook>(*(DWORD***)ClientMode, true);
	//_OverrideMouseInput = gOverrideMouseHook->Hook(23, (fnOverrideMouseHook)hkOverrideMouseInput);

	EngineClient->ClientCmd("clear");
	Msg("Hooked.\n");
	EngineClient->GetScreenSize(ScreenWidth, ScreenHeight);
	//name = pCvar->FindVar("name");
	//*(int*)((DWORD)&name->fnChangeCallback + 0xC) = NULL;

	return;
}

void unhookThread(LPARAM lpParam) {
	HMODULE hModule = (HMODULE)lpParam;
	while (true) {
		if (GetAsyncKeyState(VK_F9) & 1 && GetForegroundWindow() == FindWindow(NULL, "Counter-Strike: Global Offensive")) {
			MessageBeep(0xFFFFFFFF);
			gPanelHook->Unhook(41);
			gCreateMoveHook->Unhook(24);
			Msg("Unhooking.\n");
			Sleep(100);

			FreeLibraryAndExitThread(hModule, 0);
		}
		Sleep(100);
	}
}

int __stdcall DllMain( HMODULE Instacen, DWORD Reason, LPVOID _Reserved )
{
	if ( Reason == 1 )
	{

		MessageBeep(0xFFFFFFF);
		CreateThread( 0, 0, ( LPTHREAD_START_ROUTINE ) Start, 0, 0, 0 );
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE) unhookThread, Instacen, NULL, NULL);
		
	}
	return 1;
}
