#ifndef _GAMECORE_TYPE_DEFINITIONS_H__
#define _GAMECORE_TYPE_DEFINITIONS_H__

#ifndef DOUBLE_PRECISION
# define FLOAT_PRECISION
#endif

#include "Platform.h"

#if defined(TARGET_PLATFORM_WIN32)
# define WIN32_LEAN_AND_MEAN
# include <windows.h>
# include <windowsx.h>
#endif

#include <Math/Vector.h>
#include <Math/Rectangle.h>

typedef SDK::Math::Vector<float, 3> Vector3D;
typedef SDK::Math::Vector<float, 2> Vector2D;
typedef SDK::Math::Rectangle<float> FRect;
typedef SDK::Math::Rectangle<int> IRect;

namespace SDK
	{

	typedef unsigned short ushort;
	typedef unsigned int	 uint;
	typedef unsigned long	 ulong;
	typedef unsigned char  uchar;

	typedef unsigned __int64 uint64;
	typedef signed __int64	int64;

#ifdef FLOAT_PRECISION
	typedef float					 real;
#elif DOUBLE_PRECISION
	typedef double				 real;
#endif

	} // SDK

#endif