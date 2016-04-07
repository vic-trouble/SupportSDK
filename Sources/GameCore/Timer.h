#ifndef	__GAMECORE_TIMER_H__
#define __GAMECORE_TIMER_H__

#include "GameCoreAPI.h"

namespace SDK
{

	class Timer
	{
	private:
		__int64 m_start_time = 0;
		__int64 m_idle_ticks = 0;
		__int64 m_idle_start_time = 0;

	public:
		GAMECORE_EXPORT void Start();

		GAMECORE_EXPORT void Pause();
		GAMECORE_EXPORT void Resume();

		GAMECORE_EXPORT float GetElapsedTime() const;

		static void Sync();
	};

} // SDK

#endif