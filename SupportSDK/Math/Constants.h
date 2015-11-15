#ifndef __MATH_CONSTANTS_H__
#define __MATH_CONSTANTS_H__

namespace SDK
{
	namespace Math
	{
#if defined(CPP_11)
		constexpr float EPSILON = 0.0001f;
		constexpr float PI = 3.14593f;
		constexpr float DEG2RAD = PI / 180.f;
#else
# define EPSILON 0.0001f
# define PI 3.14593f
# define DEG2RAD PI/180.f
#endif
	} // Math
} // SDK

#endif
