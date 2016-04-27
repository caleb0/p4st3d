class ConVar
{
public:
	void SetValue(const char *value);
	void SetValue(float value);
	void SetValue(int value);
	void InternalSetString(const char* str);
	char* GetName();
	char* GetDefault();

	char pad_0x0000[0x4]; //0x0000
	ConVar* pNext; //0x0004 
	__int32 bRegistered; //0x0008 
	char* pszName; //0x000C 
	char* pszHelpString; //0x0010 
	__int32 nFlags; //0x0014 
	char pad_0x0018[0x4]; //0x0018
	ConVar* pParent; //0x001C 
	char* pszDefaultValue; //0x0020 
	char* strString; //0x0024 
	__int32 StringLength; //0x0028 
	float fValue; //0x002C 
	__int32 nValue; //0x0030 
	__int32 bHasMin; //0x0034 
	float fMinVal; //0x0038 
	__int32 bHasMax; //0x003C 
	float fMaxVal; //0x0040 
	void* fnChangeCallback; //0x0044 
};//Size=0x0048

class ICVar
{
public:
	void RegisterConCommand(ConVar *pCvar);
	ConVar* FindVar(const char *var_name);
};

void ICVar::RegisterConCommand(ConVar *pCvar)
{
	typedef void(__thiscall* OriginalFn)(void*, ConVar*);
	return EmulateVirtual<OriginalFn>(this, 1)(this, pCvar);
}

ConVar* ICVar::FindVar(const char *var_name)
{
	typedef ConVar*(__thiscall* OriginalFn)(void*, const char*);
	return EmulateVirtual<OriginalFn>(this, 15)(this, var_name);
}




void ConVar::SetValue(const char* value)
{
	typedef void(__thiscall* OriginalFn)(void*, const char*);
	return EmulateVirtual<OriginalFn>(this, 14)(this, value);
}

void ConVar::SetValue(float value)
{
	typedef void(__thiscall* OriginalFn)(void*, float);
	return EmulateVirtual<OriginalFn>(this, 15)(this, value);
}

void ConVar::SetValue(int value)
{
	typedef void(__thiscall* OriginalFn)(void*, int);
	return EmulateVirtual<OriginalFn>(this, 16)(this, value);
}
char* ConVar::GetName()
{
	typedef char*(__thiscall* OriginalFn)(void*);
	return EmulateVirtual<OriginalFn>(this, 5)(this);
}

char* ConVar::GetDefault()
{
	return pszDefaultValue;
}