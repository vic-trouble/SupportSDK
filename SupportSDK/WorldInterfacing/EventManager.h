#ifndef __EVENTMANAGEMENT_H__
#define __EVENTMANAGEMENT_H__

#include "WorldInterfacingAPI.h"

#include "TypeDefines.h"

namespace SDK
	{

	struct	EventBase;
	class		EventListener;

	class EventManager
		{
		private:
			typedef std::vector<EventListener*> Listeners;
			typedef std::vector<EventBase>			EventsPool;
		private:
			Listeners m_listeners;
			EventsPool m_events;

		protected:
			// This is used for test purposes
			//	Reason:	class is not designed for inheritance
			//					so in tests we cannot mock methods
			const Listeners& GetListeners() const
				{
				return m_listeners;
				}

		public:
			WORLDINTERFACING_API EventManager();
			WORLDINTERFACING_API ~EventManager();

			WORLDINTERFACING_API void RegisterListener(EventListener* ip_listener);
			WORLDINTERFACING_API void UnregisterListener(EventListener* ip_listener);

			WORLDINTERFACING_API void SheduleEvent(const EventBase& i_event);
			WORLDINTERFACING_API void DispatchEvents(ulong i_available_time);
		};

	} // SDK

#endif