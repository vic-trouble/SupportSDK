#ifndef _GAMECORE_TYPE_DEFINITIONS_H__
#define _GAMECORE_TYPE_DEFINITIONS_H__

#include "Platform.h"
#define NOMINMAX
#include <Math/MathDefinitions.h>

#if defined(TARGET_PLATFORM_WIN32)
# define WIN32_LEAN_AND_MEAN
# define NOMINMAX
# include <windows.h>
# include <windowsx.h>
#endif



namespace SDK
{

	typedef unsigned short	ushort;
	typedef unsigned int	uint;
	typedef unsigned long	ulong;
	typedef unsigned char	ubyte;

	typedef int				int32;

	typedef unsigned __int64	uint64;
	typedef signed __int64		int64;
} // SDK

#endif