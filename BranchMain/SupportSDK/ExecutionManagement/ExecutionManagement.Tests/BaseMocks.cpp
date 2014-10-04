#include "stdafx.h"

#include "BaseMocks.h"

using namespace SDK;

ActionRecordData::ActionRecordData(SDK::ushort i_frequency,
									 SDK::ushort i_phase,
									 SDK::real i_priority)
									 : m_frequency(i_frequency)
									 , m_phase(i_phase)
									 , m_priority(i_priority)
	{
	}

bool ActionRecordData::operator == (const ActionRecordData& i_other) const
	{
	return this->m_frequency == i_other.m_frequency
			&& this->m_phase == i_other.m_phase
			&& this->m_priority == i_other.m_priority;
	}