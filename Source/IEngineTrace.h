#pragma once

struct Ray_t
{
	Ray_t() { }

	VectorAligned		m_Start;
	VectorAligned		m_Delta;
	VectorAligned		m_StartOffset;
	VectorAligned		m_Extents;
	const Matrix3x4_t	*m_pWorldAxisTransform;
	bool				m_IsRay;
	bool				m_IsSwept;

	void Init(CVector vecStart, CVector vecEnd)
	{
		m_Delta = VectorAligned(vecEnd - vecStart);
		m_IsSwept = (m_Delta.LengthSqr() != 0);
		m_Extents.Clear();
		m_pWorldAxisTransform = NULL;
		m_IsRay = true;
		m_StartOffset.Clear();
		m_Start = vecStart;
	}
};
struct cplane_t
{
	CVector normal;
	float dist;
	BYTE type;
	BYTE signbits;
	BYTE pad[2];
};

struct csurface_t
{
	const char* name;
	short surfaceProps;
	unsigned short  flags;
};

enum TraceType_t
{
	TRACE_EVERYTHING = 0,
	TRACE_WORLD_ONLY,				// NOTE: This does *not* test static props!!!
	TRACE_ENTITIES_ONLY,			// NOTE: This version will *not* test static props
	TRACE_EVERYTHING_FILTER_PROPS,	// NOTE: This version will pass the IHandleEntity for props through the filter, unlike all other filters
};

class ITraceFilter
{
public:
	virtual bool			ShouldHitEntity(void* pEntity, int mask) = 0;
	virtual TraceType_t            GetTraceType() const = 0;
};

class CTraceFilter : public ITraceFilter
{
public:
	bool ShouldHitEntity(void* pEntityHandle, int contentsMask)
	{
		return !(pEntityHandle == pSkip1);
	}

	TraceType_t GetTraceType() const
	{
		return TRACE_EVERYTHING;
	}

	void* pSkip1;
};
class CBaseTrace
{
public:
	bool func1();
	bool func2();
	bool func3();
	bool func4();
	bool func5();
public:
	CVector startpos;
	CVector endpos;
	cplane_t plane;
	float fraction;
	int contents;
	unsigned short dispFlags;
	bool  allsolid;
	bool startsolid;
};

class CGameTrace : public CBaseTrace
{
public:
	bool DidHitWorld();
	bool DidHitNonWorldEntity();
	int GetEntityIndex();
	bool DidHit();
public:
	float fractionleftsolid;
	csurface_t surface;
	int hitgroup;
	short physicsbone;
	unsigned short worldSurfaceIndex;
	void *m_pEnt;
	int hitbox;
};
typedef CGameTrace trace_t;

class IEngineTrace
{
public:
	void TraceRay(const Ray_t &ray, unsigned int fMask, ITraceFilter* filter, trace_t *trace)
	{
		typedef void(__thiscall* fn)(void*, const Ray_t&, unsigned int, ITraceFilter*, trace_t*);
		return EmulateVirtual<fn>(this, 5)(this, ray, fMask, filter, trace);
	}
}; extern IEngineTrace * EngineTrace;
