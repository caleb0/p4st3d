#pragma once
#define DEG2RAD( x ) ( ( float )( x ) * ( float )( ( float )( M_PI ) / 180.0f ) )

#include <windows.h>
int maxEntities;
#include <intrin.h>
#include <string>
#include <Psapi.h>
#include "matrix3x4.h"
#include "poly.h"
#include <memory>

#include "hook.h"
#include "vector.h"
#include "usercmd.h"
#include "studio.h"
#include "top.h"
CUserCmd* GUserCmd = NULL;
/* sdk */

#include "ivmodelrender.h"
#include "ConVar.h"
#include "client-entity-list.h"
#include "engine-client.h"
#include "base-client.h"
#include "clientmode.h"
#include "panel.h"
#include "surface.h"
#include "IEngineTrace.h"


#define validptr(p) (p != NULL && p != nullptr)
CTools * Tools;
IPanel * Panel;
ISurface * Surface;
IClientMode ** ClientMode;
ICVar* pCvar;
IVEngineClient * EngineClient;
IEngineTrace* EngineTrace;
IBaseClientDll * BaseClientDll;
IClientEntityList * ClientEntityList;
IVModelRender * ModelRender;
IMaterial * Material;
CVRenderView * RenderView;
CModelInfo * ModelInfo;
CMaterialSystem * MaterialSystem;


/* cheat */
#include "esp.h"
#include "aim.h"
#include "misc.h"
#include "antiaim.h"

bool gIsAimbotting;
int ScreenWidth;
int ScreenHeight;
int x;
int y;
int dWidth;
int dHeight;

DWORD pClient;
CESP * ESP;
CAim * Aim;
CTrigger * Trigger;
CRender * Render;
fnPaintTraverse _PaintTraverse;
fnCreateMove _CreateMove;
fnOverrideMouseHook _OverrideMouseInput;
fnDrawModelExecute _DrawModelExecute;
static CBaseEntity * LocalPlayer;
