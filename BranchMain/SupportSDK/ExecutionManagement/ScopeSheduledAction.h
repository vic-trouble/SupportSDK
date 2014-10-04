#ifndef __SCOPEDSHEDULEDACTION_H__
#define __SCOPEDSHEDULEDACTION_H__

#include "ExecutionManagementAPI.h"

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
			void SheduleAction (std::function<void(ulong,ulong)> i_action, ushort i_frequency, ushort i_phase);

		private:
			ScopeSheduledAction (const ScopeSheduledAction& i_other);
			ScopeSheduledAction& operator = (const ScopeSheduledAction& i_other);

		public:
			EXECUTIONMANAGEMENT_API ScopeSheduledAction (ISheduler& i_sheduler, std::function<void(ulong,ulong)> i_action, ushort i_frequency, ushort i_phase);
			EXECUTIONMANAGEMENT_API ScopeSheduledAction (ScopeSheduledAction&& i_other);
			EXECUTIONMANAGEMENT_API ~ScopeSheduledAction ();
		};

	////////////////////////////////////////////////////////////////////////////////

	template <typename T>
	ScopeSheduledAction SheduleMemberFunction (ISheduler& i_sheduler, 
														T& i_object, void (T::*i_action)(ulong, ulong),
														ushort i_frequency,	ushort i_phase)
		{
		return std::move(ScopeSheduledAction(i_sheduler, ConvertMemberFunction<T>(i_object, i_action), i_frequency, i_phase));
		}

	EXECUTIONMANAGEMENT_API ScopeSheduledAction SheduleSheduler (ISheduler& i_parent_sheduler, ISheduler& i_child_sheduler,
																			 ushort i_frequency, ushort i_phase);

	} // SDK

#endif