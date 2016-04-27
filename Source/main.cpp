
#include "main.h"

int maxEntities;

static const char alphanum[] = 
"0123456789"
"!@#$%^&*"
"'.;:,/\|][{}"
"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
"abcdefghijklmnopqrstuvwxyz";
char* bSendPacket;

static float rainbow;
int stringLength = strlen(alphanum) - 1;
void getRecoil(CVector Punch) {
	x = ScreenWidth / 2;
	y = ScreenHeight / 2;
	dHeight = ScreenHeight / 90;
	dWidth = ScreenWidth / 90;
	x -= (dWidth*(Punch.y));
	y += (dHeight*(Punch.x));
}
std::string genRandom() {
	std::string Str;
	for (int i = 0; i < 20; i++) {
		Str += alphanum[rand() % stringLength];
	}
	return Str;
}
bool once = true;
void __stdcall HookedPaintTraverse( int VGUIPanel, bool ForceRepaint, bool AllowForce )
{	
	_PaintTraverse( Panel, VGUIPanel, ForceRepaint, AllowForce );
	if ( !strcmp( "FocusOverlayPanel", Panel->GetName( VGUIPanel ) ) )
	{
		if (EngineClient->IsInGame() && EngineClient->IsConnected())
		{
			ESP->Think(LocalPlayer);
			Render->DrawRect(x - 3, y - 3, 6, 6, CColor(255, 255, 255, 255));
		}
		else {
			Render->DrawRainbow(0, 0, ScreenWidth, 2, 0.003f, rainbow);
		}

	}
}
/* cmove hook function */
bool __stdcall HookedCreateMove( float SampleTime, CUserCmd* UserCmd )
{
	if ( !UserCmd->CommandNumber )
		return true;
	GUserCmd = UserCmd;
	LocalPlayer = ( CBaseEntity* ) ClientEntityList->GetClientEntity( EngineClient->GetLocalPlayer( ) );
	if ( !LocalPlayer )
		return true;

	void *vBase, *vInputBase;
	__asm mov vBase, ebp;
	vInputBase = (void*)vBase;
	bSendPacket = *(char**)vInputBase - 0x1C; // copypasted from moneybot
	*bSendPacket = TRUE;

	maxEntities = EngineClient->GetMaxClients();

	Aim->Think(LocalPlayer);
	Trigger->Think(LocalPlayer);
	if ( UserCmd->Buttons & IN_JUMP )
		if ( !( LocalPlayer->GetFlags( ) & FL_ONGROUND ) )
		{ 
			if (UserCmd->MousedX < 0) {
				UserCmd->SideMove = -450.f;
			} else if (UserCmd->MousedX > 0) {
				UserCmd->SideMove = 450.f;
			}
			UserCmd->Buttons &= ~IN_JUMP;
		}

		static ConVar* name = pCvar->FindVar("name");
		*(int*)((DWORD)&name->fnChangeCallback + 0xC) = NULL;
		if (name != NULL) {
			name->SetValue("\nVALVE");
		}
	

	getRecoil(LocalPlayer->GetPunch());
	return false;
}

/* main */
void __stdcall Start( )
{
	RUNONCE_START(asdf);
	Tools = new CTools;
	Panel = ( IPanel* )Tools->GetInterface( "vgui2.dll", "VGUI_Panel009" );
	Surface	= ( ISurface* )Tools->GetInterface( "vguimatsurface.dll", "VGUI_Surface031" );
	EngineClient = ( IVEngineClient* )Tools->GetInterface( "engine.dll", "VEngineClient013" );
	ClientEntityList = ( IClientEntityList* )Tools->GetInterface( "client.dll",	"VClientEntityList003" );
	BaseClientDll = ( IBaseClientDll* ) Tools->GetInterface( "client.dll", "VClient017" );
	pCvar = (ICVar*)Tools->GetInterface("vstdlib.dll", "VEngineCvar007");
	EngineTrace = (IEngineTrace*)Tools->GetInterface("engine.dll", "EngineTraceClient004");

	void** BaseClientDllVMT = *( void*** ) BaseClientDll;
	ClientMode = *( IClientMode*** ) ( ( DWORD ) BaseClientDllVMT[ 10 ] + 5 );

	ESP = new CESP;
	CreateMoveHook = new CHook(*(DWORD***)ClientMode);
	_CreateMove = (CreateMove)CreateMoveHook->dwHookMethod((DWORD)HookedCreateMove, 24);
	PanelHook = new CHook( ( DWORD** ) Panel );
	_PaintTraverse = ( PaintTraverse ) PanelHook->dwHookMethod( ( DWORD ) HookedPaintTraverse, 41 );

	EngineClient->ClientCmd("clear");
	Msg("Hooked.\n");
	EngineClient->GetScreenSize(ScreenWidth, ScreenHeight);
	RUNONCE_END(asdf);
	return;
}

void unhookThread(LPARAM lpParam) {
	HMODULE hModule = (HMODULE)lpParam;
	while (true) {
		if (GetAsyncKeyState(VK_F9) & 1 && GetForegroundWindow() == FindWindow(NULL, "Counter-Strike: Global Offensive")) {
			MessageBeep(0xFFFFFFFF);
			PanelHook->UnHook();
			CreateMoveHook->UnHook();
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
