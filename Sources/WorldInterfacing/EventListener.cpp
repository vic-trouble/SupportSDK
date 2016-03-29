#include "stdafx.h"

#include "EventListener.h"

#include "EventManager.h"

namespace SDK
	{

	EventListener::EventListener(EventManager* ip_manager /*= nullptr*/)
		: mp_manager(ip_manager)
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