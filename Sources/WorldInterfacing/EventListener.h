#ifndef __EVENTLISTENER_H__
#define __EVENTLISTENER_H__

#include "WorldInterfacingAPI.h"

#include "TypeDefines.h"

namespace SDK
	{

	struct	EventBase;
	class		EventManager;

	/*
	EventListener is class that provides interface for handling an event
		of specific type. It is listening only to one event at one moment.
		It is done not to use vector or set (or another data structure) and use
		m_interest_code == i_code_of_event as condition.
	*/
	
	/*
	EventListener - base class for handling events. Pass all events
		in base implementation
	*/
	class WORLDINTERFACING_API EventListener
		{
		private:
			EventManager* mp_manager;
			bool					m_is_listening;

		public:
			EventListener(EventManager* ip_manager = nullptr);
			virtual ~EventListener();

			void UnregisterInEventManager ();
			void RegisterInEventManager (EventManager* ip_manager);

			virtual bool IsCodeInteresting(const uint i_code) const;
			virtual void HandleEvent(const EventBase& i_event) = 0;
		};

	///////////////////////////////////////////////////////////////////

	inline bool EventListener::IsCodeInteresting(const uint i_code) const
		{
		return true;
		}

	} // SDK

#endif