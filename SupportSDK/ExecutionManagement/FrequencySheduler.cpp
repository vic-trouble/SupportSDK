#include "stdafx.h"

#include "FrequencySheduler.h"

#include <time.h>

namespace SDK
	{
	
	/////////////////////////////////////////////////////////////////////////////////
	// FrequencySheduler

	FrequencySheduler::FrequencySheduler()
		: m_next_id(0)
		, m_current_tick(0)
		{
		}

	FrequencySheduler::~FrequencySheduler()
		{
		
		}

	int	FrequencySheduler::AddAction(const ActionRecord& i_action)
		{
		m_actions.push_back( i_action );
		m_actions.back().m_id = m_next_id;
		return m_next_id++;
		}

	void FrequencySheduler::RemoveAction(int i_id)
		{
		auto new_end = std::remove_if(m_actions.begin(), m_actions.end(), [i_id](ActionRecord& i_record)
			{
			return i_record.m_id == i_id;
			});
		m_actions.erase(new_end, m_actions.end());
		}

	void FrequencySheduler::Execute(ulong i_available_time)
		{
		++m_current_tick;

		for (auto& action_record : m_actions)
			{
			if (m_current_tick < action_record.m_frequency)
				continue;
			if ((m_current_tick+action_record.m_phase) % action_record.m_frequency == 0)
				action_record.m_action_to_perform(i_available_time);
			}
		}

	} // SDK