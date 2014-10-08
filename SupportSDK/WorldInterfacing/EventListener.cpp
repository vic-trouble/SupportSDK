#include "stdafx.h"

#include "EventListener.h"

#include "EventManager.h"

namespace SDK
	{

	EventListener::EventListener(uint i_interest_code /*= 0*/, EventManager* ip_manager /*= nullptr*/)
		: m_interest_code (i_interest_code)
		, mp_manager(ip_manager)
		{
		if (mp_manager)
			RegisterInEventManager(mp_manager);
		}

	EventListener::~EventListener()
		{
		UnregisterInEventManager();
		}

	void EventListener::UnregisterInEventManager ()
		{
		if (mp_manager)
			{
			mp_manager->UnregisterListener(this);
			mp_manager = nullptr;
			}
		}

	void EventListener::RegisterInEventManager (EventManager* ip_manager)
		{
		mp_manager = ip_manager;
		mp_manager->RegisterListener(this);
		}

	} // SDK