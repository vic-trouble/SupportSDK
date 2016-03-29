#include "stdafx.h"

#include "EventManager.h"

#include "Event.h"
#include "EventListener.h"

#include <time.h>

namespace SDK
	{

	EventManager::EventManager()
		{
		m_events.reserve(10);
		}

	EventManager::~EventManager()
		{		}

	void EventManager::RegisterListener(EventListener* ip_listener)
		{
		if (std::find(m_listeners.begin(), m_listeners.end(), ip_listener) != m_listeners.end())
			return;
		m_listeners.push_back(ip_listener);
		}

	void EventManager::UnregisterListener(EventListener* ip_listener)
		{
		auto new_end = std::remove(m_listeners.begin(), m_listeners.end(), ip_listener);
		m_listeners.erase(new_end, m_listeners.end());
		}

	void EventManager::SheduleEvent(const EventBase& i_event)
		{
		m_events.push_back(i_event);
		}

	void EventManager::DispatchEvents(ulong i_available_time)
		{
		while (!m_events.empty())
			{
			ulong last_time = clock();

			EventBase& ev = m_events.back();

			// handle this event for all listeners
			for (auto p_listener : m_listeners)
				{
				if (p_listener->IsCodeInteresting(ev.m_code))
					p_listener->HandleEvent(ev);
				}
			// release event
			m_events.pop_back();

			// check available time
			ulong actions_time = clock() - last_time;
			if (i_available_time <= actions_time)
				break;

			i_available_time -= actions_time;
			}
		}

	} // SDK