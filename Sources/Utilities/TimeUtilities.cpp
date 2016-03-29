#include "stdafx.h"

#include "TimeUtilities.h"

#include <time.h>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif


namespace SDK
	{

	namespace Utilities
		{

		void SleepSeconds(ulong i_seconds)
			{
			SleepMiliseconds(i_seconds*1000);
			}

		void SleepMiliseconds(ulong i_miliseconds)
			{
			#ifdef _WIN32
				Sleep(i_miliseconds);
			#else
			 usleep(i_miliseconds * 1000); // takes microseconds
			#endif
			}

		} // Utilities

	} // SDK