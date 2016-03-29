#include "stdafx.h"

#include "ScopeSheduledAction.h"

#include "ActionRecord.h"
#include "ISheduler.h"

namespace SDK
	{

	ScopeSheduledAction::ScopeSheduledAction(ISheduler& i_sheduler, const ActionRecord& i_action)
		: m_sheduler (i_sheduler)
		, m_action_id (-1)
		{
		SheduleAction(i_action);
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

	void ScopeSheduledAction::SheduleAction(const ActionRecord& i_action)
		{
		m_action_id = m_sheduler.AddAction(i_action);
		}

	///////////////////////////////////////////////////////////////////////////////////////////
		
	ScopeSheduledAction SheduleSheduler (ISheduler& i_parent_sheduler, ISheduler& i_child_sheduler, ActionRecord i_action)
		{
		i_action.m_action_to_perform = ConvertShedulerToAction(i_child_sheduler);
		return std::move(ScopeSheduledAction(i_parent_sheduler, i_action));
		}

	} // SDK