#ifndef __TIMECONTROLLER_H__
#define __TIMECONTROLLER_H__

#include "GameCoreAPI.h"

namespace SDK
	{

	class TickListener
		{
		public:
			virtual void TickPerformed(long i_ms_for_tick) = 0;
		};

	class TimeController
		{
		private:
			//list of listeners that will handle message that tick is performed
			std::vector<TickListener*> m_listeners;

			unsigned long m_ticks_performed;

		public:
			GAMECORE_EXPORT TimeController();
			GAMECORE_EXPORT ~TimeController();

			GAMECORE_EXPORT void Update(long i_elapsed_time);
			GAMECORE_EXPORT void AddSubscriber(TickListener* ip_listener);
			GAMECORE_EXPORT void RemoveSubscriber(TickListener* ip_listener);

			unsigned long GetTicksPerformed() const;
		};

	inline unsigned long TimeController::GetTicksPerformed() const
		{
		return m_ticks_performed;
		}

	} // SDK

#endif