#include "stdafx.h"

#include "EventManager.h"
#include "EventListenerBroadCasting.h"

namespace SDK
	{

	EventListenerBroadCasting::EventListenerBroadCasting(EventManager* ip_manager /*=nullptr*/)
		: EventListener(ip_manager)
		, m_initized(false)
		{		}

	EventListenerBroadCasting::~EventListenerBroadCasting()
		{		}

	void EventListenerBroadCasting::AddCode(uint i_code)
		{
		// code is already in vector
		if (IsCodeInteresting(i_code))
			return;

		m_intetest_codes.push_back(i_code);
		if (m_initized)
			std::sort (m_intetest_codes.begin(), m_intetest_codes.end());
		}

	void EventListenerBroadCasting::Flush()
		{
		m_intetest_codes.clear();
		}

	void EventListenerBroadCasting::Initialize()
		{
		std::sort (m_intetest_codes.begin(), m_intetest_codes.end());
		m_initized = true;
		}

	bool EventListenerBroadCasting::IsCodeInteresting(const uint i_code) const
		{
		return m_intetest_codes.end() != std::find(m_intetest_codes.begin(), m_intetest_codes.end(), i_code);
		}

	} // SDK