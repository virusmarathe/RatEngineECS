#pragma once
#pragma once
#include <ostream>
#include "Vector3.h"

class Vector4
{
public:
	Vector4() : x(0), y(0), z(0), w(0)
	{
	}

	Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w)
	{
	}

	Vector4(const Vector4& vec) : x(vec.x), y(vec.y), z(vec.z), w(vec.w)
	{
	}

	Vector4(const Vector3& vec) : x(vec.x), y(vec.y), z(vec.z), w(0)
	{
	}

	~Vector4()
	{
	}

	void normalize()
	{
		float magVal = mag();
		if (magVal > 0)
		{
			x /= magVal;
			y /= magVal;
			z /= magVal;
		}
	}

	float magSqr()
	{
		return x * x + y * y + z * z;
	}

	float mag()
	{
		return sqrt(magSqr());
	}

	// ab cos(theta)
	float dot(const Vector4& vec)
	{
		return (x * vec.x) + (y * vec.y) + (z * vec.z);
	}

	Vector4 operator+(const Vector4& rhs)
	{
		return Vector4(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
	}

	Vector4 operator-(const Vector4& rhs)
	{
		return Vector4(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
	}

	Vector4 operator*(float val)
	{
		return Vector4(x * val, y * val, z * val, w * val);
	}

	void cross(Vector4& v1, Vector4& v2, Vector4& v3)
	{
		this->x = v1.y * (v2.z * v3.w - v3.z * v2.w) - v1.z * (v2.y * v3.w - v3.y * v2.w) + v1.w * (v2.y * v3.z - v2.z * v3.y);
		this->y = -(v1.x * (v2.z * v3.w - v3.z * v2.w) - v1.z * (v2.x * v3.w - v3.x * v2.w) + v1.w * (v2.x * v3.z - v3.x * v2.z));
		this->z = v1.x * (v2.y * v3.w - v3.y * v2.w) - v1.y * (v2.x * v3.w - v3.x * v2.w) + v1.w * (v2.x * v3.y - v3.x * v2.y);
		this->w = -(v1.x * (v2.y * v3.z - v3.y * v2.z) - v1.y * (v2.x * v3.z - v3.x * v2.z) + v1.z * (v2.x * v3.y - v3.x * v2.y));
	}

	static Vector4 lerp(Vector4 start, Vector4 end, float lerpVal)
	{
		return (start * (1 - lerpVal)) + (end * lerpVal);
	}

public:
	float x, y, z, w;

	friend std::ostream& operator<<(std::ostream& os, const Vector4& vec);
};

inline std::ostream& operator<<(std::ostream& os, const Vector4& vec)
{
	os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
	return os;
}