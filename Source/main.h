#include <windows.h>
#include <intrin.h>
#include "top.h"
#include "poly.h"
#include "math.h"
#include "vector.h"
/* sdk */
#include "ConVar.h"
#include "client-entity-list.h"
#include "engine-client.h"
#include "base-client.h"
#include "clientmode.h"
#include "panel.h"
#include "surface.h"
#include "hook.h"
#include "usercmd.h"
#include "IEngineTrace.h"

#define validptr(p) (p != NULL && p != nullptr)
CUserCmd* GUserCmd = NULL;
CTools * Tools;
IPanel * Panel;
ISurface * Surface;
IClientMode ** ClientMode;
ICVar* pCvar;
IVEngineClient * EngineClient;
IEngineTrace* EngineTrace;
IBaseClientDll * BaseClientDll;
IClientEntityList * ClientEntityList;

/* cheat */
#include "esp.h"
#include "aim.h"
#include "location.h"

int ScreenWidth;
int ScreenHeight;
int x;
int y;
int dWidth;
int dHeight;
CHook * PanelHook;
CHook * CreateMoveHook;
DWORD pClient;
CESP * ESP;
CAim * Aim;
CTrigger * Trigger;
CRender * Render;
PaintTraverse _PaintTraverse;
CreateMove _CreateMove;
static CBaseEntity * LocalPlayer;