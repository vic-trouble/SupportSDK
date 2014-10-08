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

	class WORLDINTERFACING_API EventListener
		{
		private:
			EventManager* mp_manager;
			uint					m_interest_code;
			bool					m_is_listening;

		public:
			EventListener(uint i_interest_code = 0, EventManager* ip_manager = nullptr);
			virtual ~EventListener();

			void UnregisterInEventManager ();
			void RegisterInEventManager (EventManager* ip_manager);

			void SetInterestCode (uint i_code);
			bool IsCodeInteresting(const uint i_code) const;

			virtual void HandleEvent(const EventBase& i_event) = 0;
		};

	///////////////////////////////////////////////////////////////////

	inline void EventListener::SetInterestCode (uint i_code)
		{
		m_interest_code = i_code;
		}

	inline bool EventListener::IsCodeInteresting(const uint i_code) const
		{
		return i_code == m_interest_code;
		}

	} // SDK

#endif