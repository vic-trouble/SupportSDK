#include "stdafx.h"

#include "TimeUtilities.h"

#include <time.h>

namespace SDK
	{

	namespace Utilities
		{

		void SleepSeconds(ulong i_seconds)
			{
			clock_t now = clock();
			clock_t limit = now + i_seconds * CLOCKS_PER_SEC;
			while ( limit > now )
				now = clock();
			}

		void SleepMiliseconds(ulong i_miliseconds)
			{
			long timeout = clock() + i_miliseconds;
			while( clock() < timeout ) continue;
			}

		} // Utilities

	} // SDK