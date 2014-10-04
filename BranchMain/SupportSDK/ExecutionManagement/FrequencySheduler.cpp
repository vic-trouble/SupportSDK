#include "stdafx.h"

#include "FrequencySheduler.h"

#include <time.h>

namespace SDK
	{
	/////////////////////////////////////////////////////////////////////////////////
	// ActionRecord

	FrequencySheduler::ActionRecord::ActionRecord(int i_id, std::function<void(ulong, ulong)> i_action, ushort i_frequency, ushort i_phase)
		: m_id(i_id)
		, m_action_to_perform(i_action)
		, m_frequency(i_frequency)
		, m_phase(i_phase)
		{
		}

	/////////////////////////////////////////////////////////////////////////////////
	// FrequencySheduler

	FrequencySheduler::FrequencySheduler()
		: m_next_id(0)
		, m_current_tick(0)
		, m_general_time(0)
		, m_last_time(0)
		{
		}

	FrequencySheduler::~FrequencySheduler()
		{
		
		}

	int	FrequencySheduler::AddAction(std::function<void(ulong, ulong)> i_action, ushort i_frequency, ushort i_phase)
		{
		m_actions.push_back( ActionRecord(m_next_id, i_action, i_frequency, i_phase) );
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
		
		ulong difference = 0;
		if (m_last_time == 0)
			m_last_time = clock();
		else
			difference = clock() - m_last_time;
		m_general_time += difference;

		for (auto& action_record : m_actions)
			{
			if (m_current_tick < action_record.m_frequency)
				continue;
			if ((m_current_tick+action_record.m_phase) % action_record.m_frequency == 0)
				action_record.m_action_to_perform(difference, i_available_time);
			}

		// update last time
		m_last_time = clock();
		}

	} // SDK