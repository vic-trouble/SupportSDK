#include "stdafx.h"

#include "ScopeSheduledAction.h"
#include "ISheduler.h"

namespace SDK
	{

	ScopeSheduledAction::ScopeSheduledAction(ISheduler& i_sheduler, std::function<void(ulong,ulong)> i_action, ushort i_frequency, ushort i_phase)
		: m_sheduler (i_sheduler)
		, m_action_id (-1)
		{
		SheduleAction(i_action, i_frequency, i_phase);
		}

	ScopeSheduledAction::ScopeSheduledAction (ScopeSheduledAction&& i_other)
		: m_sheduler(i_other.m_sheduler)
		, m_action_id(i_other.m_action_id)
		{
		i_other.m_action_id = -1;
		}

	ScopeSheduledAction::~ScopeSheduledAction()
		{
		if (m_action_id != -1)
			m_sheduler.RemoveAction(m_action_id);
		}

	void ScopeSheduledAction::SheduleAction(std::function<void(ulong,ulong)> i_action, ushort i_frequency, ushort i_phase)
		{
		m_action_id = m_sheduler.AddAction(i_action, i_frequency, i_phase);
		}

	///////////////////////////////////////////////////////////////////////////////////////////
		
	ScopeSheduledAction SheduleSheduler (ISheduler& i_parent_sheduler, ISheduler& i_child_sheduler,
																			 ushort i_frequency, ushort i_phase)
		{
		return std::move(ScopeSheduledAction(i_parent_sheduler, ConvertShedulerToAction(i_child_sheduler), i_frequency, i_phase));
		}

	} // SDK