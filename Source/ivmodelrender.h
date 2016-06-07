
enum OverrideType_t
{
	OVERRIDE_NORMAL = 0,
	OVERRIDE_BUILD_SHADOWS,
	OVERRIDE_DEPTH_WRITE,
};

enum MaterialVarFlags_t
{
	MATERIAL_VAR_DEBUG = (1 << 0),
	MATERIAL_VAR_NO_DEBUG_OVERRIDE = (1 << 1),
	MATERIAL_VAR_NO_DRAW = (1 << 2),
	MATERIAL_VAR_USE_IN_FILLRATE_MODE = (1 << 3),

	MATERIAL_VAR_VERTEXCOLOR = (1 << 4),
	MATERIAL_VAR_VERTEXALPHA = (1 << 5),
	MATERIAL_VAR_SELFILLUM = (1 << 6),
	MATERIAL_VAR_ADDITIVE = (1 << 7),
	MATERIAL_VAR_ALPHATEST = (1 << 8),
	MATERIAL_VAR_MULTIPASS = (1 << 9),
	MATERIAL_VAR_ZNEARER = (1 << 10),
	MATERIAL_VAR_MODEL = (1 << 11),
	MATERIAL_VAR_FLAT = (1 << 12),
	MATERIAL_VAR_NOCULL = (1 << 13),
	MATERIAL_VAR_NOFOG = (1 << 14),
	MATERIAL_VAR_IGNOREZ = (1 << 15),
	MATERIAL_VAR_DECAL = (1 << 16),
	MATERIAL_VAR_ENVMAPSPHERE = (1 << 17),
	MATERIAL_VAR_NOALPHAMOD = (1 << 18),
	MATERIAL_VAR_ENVMAPCAMERASPACE = (1 << 19),
	MATERIAL_VAR_BASEALPHAENVMAPMASK = (1 << 20),
	MATERIAL_VAR_TRANSLUCENT = (1 << 21),
	MATERIAL_VAR_NORMALMAPALPHAENVMAPMASK = (1 << 22),
	MATERIAL_VAR_NEEDS_SOFTWARE_SKINNING = (1 << 23),
	MATERIAL_VAR_OPAQUETEXTURE = (1 << 24),
	MATERIAL_VAR_ENVMAPMODE = (1 << 25),
	MATERIAL_VAR_SUPPRESS_DECALS = (1 << 26),
	MATERIAL_VAR_HALFLAMBERT = (1 << 27),
	MATERIAL_VAR_WIREFRAME = (1 << 28),

	// NOTE: Only add flags here that either should be read from
	// .vmts or can be set directly from client code. Other, internal
	// flags should to into the flag enum in IMaterialInternal.h
};

class IMaterial
{
public:
	const char* GetName()
	{
		typedef const char*(__thiscall* oGetName)(PVOID);
		return EmulateVirtual< oGetName >(this, 0)(this);
	}

	void SetMaterialVarFlag(MaterialVarFlags_t flag, bool value)
	{
		typedef void(__thiscall* oSetMatFlag)(PVOID, MaterialVarFlags_t, bool);
		return EmulateVirtual< oSetMatFlag >(this, 30)(this, flag, value);
	}

	bool GetMaterialVarFlag(MaterialVarFlags_t flag)
	{
		typedef bool(__thiscall* oGetMatFlag)(PVOID, MaterialVarFlags_t);
		return EmulateVirtual< oGetMatFlag >(this, 31)(this, flag);
	}

	void AlphaModulate(float a)
	{
		typedef void(__thiscall* oAlphaModulate)(PVOID, float);
		return EmulateVirtual< oAlphaModulate >(this, 28)(this, a);
	}

	void ColorModulate(float r, float g, float b)
	{
		typedef void(__thiscall* oColorModulate)(PVOID, float, float, float);
		return EmulateVirtual< oColorModulate >(this, 29)(this, r, g, b);
	}

	void IncrementReferenceCount(void)
	{
		typedef void(__thiscall* oIncrementReferenceCount)(PVOID);
		return EmulateVirtual< oIncrementReferenceCount >(this, 14)(this);
	}
}; extern IMaterial * Material;

class IVModelRender
{
public:
	void ForcedMaterialOverride(IMaterial *material)
	{
		typedef void(__thiscall* Fn)(PVOID, IMaterial*, int, int);
		return EmulateVirtual<Fn>(this, 1)(this, material, 0, 0);
	}
	void DrawModelExecute(void* ctx, void *state, const ModelRenderInfo_t &pInfo, Matrix3x4_t *pCustomBoneToWorld)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, void* ctx, void *state, const ModelRenderInfo_t &pInfo, Matrix3x4_t *pCustomBoneToWorld);
		return EmulateVirtual<OriginalFn>(this, 21)(this, ctx, state, pInfo, pCustomBoneToWorld);
	}
	
}; extern IVModelRender * ModelRender;


class CModelInfo
{
public:
	int	GetModelIndex(const char *name)
	{
		typedef int(__thiscall* oGetModelName)(PVOID, const char *);
		return EmulateVirtual< oGetModelName >(this, 2)(this, name);
	}
	const char *GetModelName(const model_t *mod)
	{
		typedef const char *(__thiscall* oGetModelName)(PVOID, const model_t*);
		return EmulateVirtual< oGetModelName >(this, 3)(this, mod);
	}

	studiohdr_t	*GetStudiomodel(const model_t *mod)
	{
		typedef studiohdr_t *(__stdcall* oGetStudiomodel)(const model_t*);
		return EmulateVirtual< oGetStudiomodel >(this, 30)(mod);
	}
}; extern CModelInfo * ModelInfo;


class CVRenderView
{
public:
	void SetBlend(float alpha)
	{
		typedef void(__thiscall* oDrawModelExecute)(PVOID, float);
		return EmulateVirtual< oDrawModelExecute >(this, 4)(this, alpha);
	}

	void SetColorModulation(float const* colors)
	{
		typedef void(__thiscall* oDrawModelExecute)(PVOID, float const*);
		return EmulateVirtual< oDrawModelExecute >(this, 6)(this, colors);
	}
}; extern CVRenderView * RenderView;


class CMaterialSystem
{
public:
	IMaterial* FindMaterial(char const* pMaterialName, const char *pTextureGroupName, bool complain = true, const char *pComplainPrefix = NULL)
	{
		typedef IMaterial*(__thiscall* oFindMaterial)(PVOID, char const*, char const*, bool, char const*);
		return EmulateVirtual< oFindMaterial >(this, 84)(this, pMaterialName, pTextureGroupName, complain, pComplainPrefix);
	}
	int created = 0;
	IMaterial* CreateMaterial(bool flat, bool ignorez, bool wireframed)
	{
		
	}

}; extern CMaterialSystem * MaterialSystem;

void ForceMaterial(IMaterial* material, CColor color)
{
	if (material != NULL)
	{
		RenderView->SetColorModulation(color.Base());
		ModelRender->ForcedMaterialOverride(material);
	}
}
