#include "stdafx.h"

#include "EventManager.h"
#include "EventListenerNarrowCasting.h"

namespace SDK
	{

	EventListenerNarrowCasting::EventListenerNarrowCasting (uint i_interest_code_mask /*= 0*/, EventManager* ip_manager /*= nullptr*/)
		: EventListener(ip_manager)
		, m_interest_code_mask(i_interest_code_mask)
		{		}

	EventListenerNarrowCasting::~EventListenerNarrowCasting()
		{		}

	void EventListenerNarrowCasting::FlushMask()
		{
		m_interest_code_mask = 0;
		}

	bool EventListenerNarrowCasting::IsCodeInteresting(const uint i_code) const
		{
		return (m_interest_code_mask & i_code) != 0;
		}

	} // SDK