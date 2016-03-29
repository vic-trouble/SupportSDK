#ifndef __SCOPEDSHEDULEDACTION_H__
#define __SCOPEDSHEDULEDACTION_H__

#include "ExecutionManagementAPI.h"

#include "ActionRecord.h"
#include "TypeDefines.h"

namespace SDK
	{

	class ISheduler;

	class ScopeSheduledAction
		{
		private:
			ISheduler&												m_sheduler;
			int																m_action_id;

		private:
			void SheduleAction (const ActionRecord& i_action);

		private:
			ScopeSheduledAction (const ScopeSheduledAction& i_other);
			ScopeSheduledAction& operator = (const ScopeSheduledAction& i_other);

		public:
			EXECUTIONMANAGEMENT_API ScopeSheduledAction (ISheduler& i_sheduler, const ActionRecord& i_action);
			EXECUTIONMANAGEMENT_API ScopeSheduledAction (ScopeSheduledAction&& i_other);
			EXECUTIONMANAGEMENT_API ~ScopeSheduledAction ();
		};

	////////////////////////////////////////////////////////////////////////////////

	template <typename T>
	ScopeSheduledAction SheduleMemberFunction (ISheduler& i_sheduler, 
														T& i_object, void (T::*i_member)(ulong),
														ActionRecord i_action)
		{
		i_action.m_action_to_perform = ConvertMemberFunction<T>(i_object, i_member);
		return std::move(ScopeSheduledAction(i_sheduler, i_action));
		}

	EXECUTIONMANAGEMENT_API ScopeSheduledAction SheduleSheduler (ISheduler& i_parent_sheduler, ISheduler& i_child_sheduler, ActionRecord i_action);

	} // SDK

#endif