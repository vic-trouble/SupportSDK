#include "stdafx.h"

#include "LoadBalancingSheduler.h"

#include <time.h>

namespace SDK
	{

	/////////////////////////////////////////////////////////////////////////////////
	// ActionRecord

	LoadBalancingSheduler::ActionRecord::ActionRecord(int i_id, std::function<void(ulong, ulong)> i_action, ushort i_frequency, ushort i_phase)
		: m_id(i_id)
		, m_action_to_perform(i_action)
		, m_frequency(i_frequency)
		, m_phase(i_phase)
		, m_last_time_run(0)
		{
		}

	/////////////////////////////////////////////////////////////////////////////////
	// FrequencySheduler

	LoadBalancingSheduler::LoadBalancingSheduler()
		: m_next_id(0)
		, m_current_tick(0)
		{
		}

	LoadBalancingSheduler::~LoadBalancingSheduler()
		{
		}

	int	LoadBalancingSheduler::AddAction(std::function<void(ulong, ulong)> i_action, ushort i_frequency, ushort i_phase)
		{
		m_actions.push_back( ActionRecord(m_next_id, i_action, i_frequency, i_phase) );
		return m_next_id++;
		}

	void LoadBalancingSheduler::RemoveAction(int i_id)
		{
		auto new_end = std::remove_if(m_actions.begin(), m_actions.end(), [i_id](ActionRecord& i_record)
			{
			return i_record.m_id == i_id;
			});
		m_actions.erase(new_end, m_actions.end());
		}

	void LoadBalancingSheduler::Execute(ulong i_available_time)
		{
		++m_current_tick;		

		std::vector<ActionRecord*> actions_to_run;
		for (auto& action_record : m_actions)
			{
			if (m_current_tick < action_record.m_frequency)
				continue;
			if ((m_current_tick + action_record.m_phase) % action_record.m_frequency == 0)
				actions_to_run.push_back(&action_record);
			}

		ulong last_time_in_tick = clock();
		const size_t actions_size = actions_to_run.size();
		for (size_t i = 0; i < actions_size; ++i)
			{
			auto& action_record = *actions_to_run.at(i);
			// find available time
			ulong current_time = clock();
			i_available_time -= current_time - last_time_in_tick;

			ulong available_time = i_available_time / (actions_size - i );
			action_record.m_action_to_perform(current_time - action_record.m_last_time_run, available_time);
			action_record.m_last_time_run = current_time;

			// update last time for this tick
			last_time_in_tick = clock();
			}
		}

	} // SDK