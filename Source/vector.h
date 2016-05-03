#pragma once

#include "top.h"

#define M_PI 3.14159265358979323846f


class CVector
{
public:

	float x, y, z;

	CVector( float x = 0, float y = 0, float z = 0 )
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}
	void Init(float _x = 0, float _y = 0, float _z = 0)
	{
		this->x = _x;
		this->y = _y;
		this->z = _z;
	}

	float& operator[](int i)
	{
		return ((float*)this)[i];
	}
	float operator[](int i) const
	{
		return ((float*)this)[i];
	}


	CVector operator + ( const CVector& In )
	{
		return CVector( x + In.x, y + In.y, z + In.z );
	}

	bool operator == (const CVector& In)
	{
		return (x == In.x && y == In.y && z == In.z);
	}
	
	CVector operator - ( const CVector& In )
	{
		return CVector( x - In.x, y - In.y, z - In.z );
	}
	CVector operator * (const CVector& In)
	{
		return CVector(x * In.x, y * In.y, z * In.z);
	}
	void operator -= ( const CVector In )
	{
		*this = CVector( x - In.x, y - In.y, z - In.z );
	}
	void Clear()
	{
		this->x = this->y = this->z = .0f;
	}

	float Length()
	{
		return sqrtf(this->x * this->x + this->y * this->y + this->z * this->z);
	}

	float LengthSqr() const
	{
		return (this->x * this->x + this->y * this->y + this->z * this->z);
	}
};

class Angle : public CVector
{
public:
	Angle(float _x = 0.f, float _y = 0.f, float _z = 0.f)
	{
		CVector(_x, _y, _z);
	}
	void Init(float x = 0, float y = 0, float z = 0)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}
	float& operator[](int i)
	{
		return ((float*)this)[i];
	}

	float operator[](int i) const
	{
		return ((float*)this)[i];
	}

	Angle operator+(const Angle& in)
	{
		return Angle(this->x + in.x, this->y + in.y, this->z + in.z);
	}

	Angle operator-(const Angle& in)
	{
		return Angle(this->x - in.x, this->y - in.y, this->z - in.z);
	}

	Angle operator*(const Angle& in)
	{
		return Angle(this->x * in.x, this->y * in.y, this->z * in.z);
	}

	Angle operator/(const Angle& in)
	{
		return Angle(this->x / in.x, this->y / in.y, this->z / in.z);
	}

	Angle operator-=(const Angle& in)
	{
		this->x -= in.x;
		this->y -= in.y;
		this->z -= in.z;

		return *this;
	}

	Angle operator+=(const Angle& in)
	{
		this->x += in.x;
		this->y += in.y;
		this->z += in.z;

		return *this;
	}

	Angle operator/=(const Angle& in)
	{
		this->x /= in.x;
		this->y /= in.y;
		this->z /= in.z;

		return *this;
	}

	Angle operator*=(const Angle& in)
	{
		this->x *= in.x;
		this->y *= in.y;
		this->z *= in.z;

		return *this;
	}

	void Clear()
	{
		this->x = this->y = this->z = .0f;
	}

	Angle Normalize()
	{
		if (this->x != this->x)
			this->x = 0;
		if (this->y != this->y)
			this->y = 0;
		if (this->z != this->z)
			this->z = 0;

		if (this->x > 89.f)
			this->x = 89.f;
		if (this->x < -89.f)
			this->x = -89.f;

		while (this->y > 180)
			this->y -= 360;
		while (this->y <= -180)
			this->y += 360;

		if (this->y > 180.f)
			this->y = 180.f;
		if (this->y < -180.f)
			this->y = -180.f;

		this->z = 0;

		return *this;
	}

	float Difference(Angle angDestination)
	{
		bool bX180 = false;
		bool bY180 = false;

		float XDiff = _Normalize(this->x - angDestination.x);
		float YDiff = _Normalize(this->y - angDestination.y);

		bX180 = 180 > XDiff;
		bY180 = 180 > YDiff;

		if (!bX180)
			XDiff -= 360;

		if (!bY180)
			YDiff -= 360;

		if (0 > XDiff)
			XDiff = (XDiff - XDiff) - XDiff;

		if (0 > YDiff)
			YDiff = (YDiff - YDiff) - YDiff;

		float Diff = YDiff + XDiff;

		return Diff;
	}
	void ClampAngles(Angle &angles)
	{
		if (angles.x > 89.f)
			angles.x -= 360.f;
		else if (angles.x < -89.f)
			angles.x += 360.f;
		if (angles.y > 180.f)
			angles.y -= 360.f;
		else if (angles.y < -180.f)
			angles.y += 360.f;

		angles.z = 0;
	}
	/*
	nigga that shit broken af
	float Difference(Angle dest)
	{
	float _x = DEG2RAD(this->x);
	float _y = DEG2RAD(this->y);
	float __x = DEG2RAD(dest.x);
	float __y = DEG2RAD(dest.y);

	Vector a = Vector(sin(_x) * cos(_y), sin(_x) * sin(_y), cos(_x));
	Vector b = Vector(sin(__x) * cos(__y), sin(__x) * sin(__y), cos(__x));

	return RAD2DEG(acos(a.x * b.x + a.y * b.y + a.z * b.z));
	}
	*/
private:
	float _Normalize(float angAngle)
	{
		return (float)(fmod(angAngle + 180, 360) - 180);
	}
	
};
void VectorAngles3D(const CVector&vecForward, CVector&vecAngles)
{
	CVector vecView;
	if (vecForward.y == 0.f && vecForward.x == 0.f)
	{
		vecView.x = 0.f;
		vecView.y = 0.f;
	}
	else
	{
		vecView.y = atan2(vecForward.y, vecForward.x) * 180.f / M_PI;

		if (vecView.y < 0.f)
			vecView.y += 360;

		vecView.z = sqrt(vecForward.x * vecForward.x + vecForward.y * vecForward.y);

		vecView.x = atan2(vecForward.z, vecView.z) * 180.f / M_PI;
	}

	vecAngles.x = -vecView.x;
	vecAngles.y = vecView.y;
	vecAngles.z = 0.f;
}

class VectorAligned : public CVector
{
public:
	VectorAligned() {}

	VectorAligned(const CVector &vec)
	{
		this->x = vec.x;
		this->y = vec.y;
		this->z = vec.z;
	}

	float w;
};
void AngleVectors(const CVector angles, float* forward, float* right, float* up)
{
	float angle;
	static float sp, sy, cp, cy;

	angle = angles[0] * (M_PI / 180.f);
	sp = sin(angle);
	cp = cos(angle);

	angle = angles[1] * (M_PI / 180.f);
	sy = sin(angle);
	cy = cos(angle);

	if (forward)
	{
		forward[0] = cp * cy;
		forward[1] = cp * sy;
		forward[2] = -sp;
	}
	if (right || up)
	{
		static float sr, cr;

		angle = angles[2] * (M_PI / 180.f);
		sr = sin(angle);
		cr = cos(angle);

		if (right)
		{
			right[0] = -1 * sr * sp * cy + -1 * cr * -sy;
			right[1] = -1 * sr * sp * sy + -1 * cr *cy;
			right[2] = -1 * sr * cp;
		}
		if (up)
		{
			up[0] = cr * sp *cy + -sr * -sy;
			up[1] = cr * sp *sy + -sr * cy;
			up[2] = cr * cp;
		}
	}
}