#pragma once
#include <ostream>

class Vector3
{
public:
	Vector3() : x(0), y(0), z(0) 
	{
	}

	Vector3(float x, float y, float z) : x(x), y(y), z(z)	
	{
	}

	Vector3(const Vector3& vec) : x(vec.x), y(vec.y), z(vec.z) 
	{
	}

	~Vector3() 
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
	float dot(const Vector3& vec)
	{
		return (x * vec.x) + (y * vec.y) + (z * vec.z);
	}

	// ab sin(theta)
	Vector3 cross(const Vector3& vec)
	{
		return Vector3( y * vec.z - z * vec.y,
					    z * vec.x - x * vec.z,
					    x * vec.y - y * vec.x);
	}

	Vector3 operator+(const Vector3& rhs)
	{
		return Vector3(x + rhs.x, y + rhs.y, z + rhs.z);
	}

	Vector3 operator-(const Vector3& rhs)
	{
		return Vector3(x - rhs.x, y - rhs.y, z - rhs.z);
	}

	Vector3 operator*(float val)
	{
		return Vector3(x * val, y * val, z * val);
	}

	static Vector3 lerp(Vector3 start, Vector3 end, float lerpVal)
	{
		return (start * (1 - lerpVal)) + (end * lerpVal);
	}

public:
	float x, y, z;

	friend std::ostream& operator<<(std::ostream& os, const Vector3& vec);
};

inline std::ostream& operator<<(std::ostream& os, const Vector3& vec)
{
	os << "(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
	return os;
}