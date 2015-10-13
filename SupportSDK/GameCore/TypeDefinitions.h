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
#include <Math/Matrix.h>
#include <Math/Quaternion.h>
#include <Math/Line.h>

namespace SDK
{

	typedef unsigned short	ushort;
	typedef unsigned int	uint;
	typedef unsigned long	ulong;
	typedef unsigned char	uchar;

	typedef int				int32;

	typedef unsigned __int64	uint64;
	typedef signed __int64		int64;

#ifdef FLOAT_PRECISION
	typedef float					 real;
#elif DOUBLE_PRECISION
	typedef double				 real;
#endif

	typedef Math::Matrix<real, 3, 3> Matrix3;

	typedef Math::Vector<real, 2> Vector2;
	typedef Math::Vector<real, 3> Vector3;
	typedef Math::Vector<real, 4> Vector4;

	typedef Math::Quaternion<real> QuaternionF;

	typedef Math::Rectangle<real> FRect;
	typedef Math::Rectangle<int> IRect;

	typedef Math::Line<real, 2> Point2F;
	typedef Math::Line<real, 3> Point3F;

} // SDK

#endif