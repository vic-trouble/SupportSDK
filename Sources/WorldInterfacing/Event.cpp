#include "stdafx.h"

#include "Event.h"

namespace SDK
	{

	EventBase::EventBase (uint i_code, void* ip_data /*= nullptr*/)
		: m_code(i_code)
		, mp_data(ip_data)
		{		}

	EventBase::~EventBase()
		{		}

	} // SDK