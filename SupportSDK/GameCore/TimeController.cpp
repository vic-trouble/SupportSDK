#include "stdafx.h"

#include "TimeController.h"

namespace SDK
	{

	TimeController::TimeController()
		: m_ticks_performed(0)
		{

		}

	TimeController::~TimeController()
		{
		m_listeners.clear();
		}

	void TimeController::Update(long i_elapsed_time)
		{
		for (TickListener* p_listener : m_listeners)
			p_listener->TickPerformed(i_elapsed_time);

		++m_ticks_performed;
		}

	void TimeController::AddSubscriber(TickListener* ip_listener)
		{
		assert(ip_listener && "<STimeController::AddSubscriber>:listener is NULL");
		m_listeners.push_back(ip_listener);
		}

	void TimeController::RemoveSubscriber(TickListener* ip_listener)
		{
		auto it_end = std::remove(m_listeners.begin(), m_listeners.end(), ip_listener);
		m_listeners.erase(it_end, m_listeners.end());
		}

	} // SDK