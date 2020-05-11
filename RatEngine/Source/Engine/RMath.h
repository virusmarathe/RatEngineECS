#pragma once

struct RMath
{
	template<typename T>
	static inline T max(const T& val1, const T& val2)
	{
		return val1 >= val2 ? val1 : val2;
	}
};