#pragma once
#include <ostream>

class Vector2
{
public:
	Vector2() : x(0), y(0)
	{
	}

	Vector2(float x, float y) : x(x), y(y)
	{
	}

	Vector2(const Vector2& vec) : x(vec.x), y(vec.y)
	{
	}

	~Vector2()
	{
	}

	Vector2 operator+(const Vector2& rhs)
	{
		return Vector2(x + rhs.x, y + rhs.y);
	}

	Vector2 operator-(const Vector2& rhs)
	{
		return Vector2(x - rhs.x, y - rhs.y);
	}

	Vector2 operator*(float val)
	{
		return Vector2(x * val, y * val);
	}

	static Vector2 lerp(Vector2 start, Vector2 end, float lerpVal)
	{
		return (start * (1 - lerpVal)) + (end * lerpVal);
	}

public:
	float x, y;

	friend std::ostream& operator<<(std::ostream& os, const Vector2& vec);
};

inline std::ostream& operator<<(std::ostream& os, const Vector2& vec)
{
	os << "(" << vec.x << ", " << vec.y << ")";
	return os;
}