#include "stdafx.h"

#include "ActionRecord.h"

#include "ISheduler.h"

namespace SDK
	{

	/////////////////////////////////////////////////////////////////////////////////
	// ActionRecord

	ActionRecord::ActionRecord(std::function<void(ulong)> i_action, 
														ushort i_frequency, 
														ushort i_phase, 
														real i_priority)
														: m_id(-1)
														, m_action_to_perform(i_action)
														, m_frequency(i_frequency)
														, m_phase(i_phase)
														, m_priority(i_priority)
		{
		}

	ActionRecord::ActionRecord (ISheduler& i_sheduler,
															ushort i_frequency,
															ushort i_phase,
															real i_priority)
														: m_id(-1)
														, m_action_to_perform(ConvertShedulerToAction(i_sheduler))
														, m_frequency(i_frequency)
														, m_phase(i_phase)
														, m_priority(i_priority)
		{
		}

	ActionRecord::ActionRecord (ushort i_frequency,
															ushort i_phase,
															real i_priority)
															: m_id(-1)
															, m_frequency(i_frequency)
															, m_phase(i_phase)
															, m_priority(i_priority)
		{
		}

	/////////////////////////////////////////////////////////////////////////////////
	// utility functions

	std::function<void(ulong)> ConvertShedulerToAction (ISheduler& i_child_sheduler)
		{
		return std::bind(&ISheduler::Execute, std::ref(i_child_sheduler), std::placeholders::_1);
		}


	} // SDK