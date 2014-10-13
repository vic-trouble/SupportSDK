#ifndef __TIMEUTILITIES_H__
#define __TIMEUTILITIES_H__

#include "UtilitiesAPI.h"
#include "TypeDefines.h"

namespace SDK
	{

	namespace Utilities
		{
		
		UTILITIES_API void SleepSeconds(ulong i_seconds);
		UTILITIES_API void SleepMiliseconds(ulong i_miliseconds);

		} // Utilities

	} // SDK

#endif