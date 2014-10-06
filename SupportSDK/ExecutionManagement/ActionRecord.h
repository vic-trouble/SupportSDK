#ifndef __ACTIONRECORD_H__
#define __ACTIONRECORD_H__

#include "ExecutionManagementAPI.h"

#include "TypeDefines.h"

namespace SDK
	{

	class ISheduler;

	struct ActionRecord
		{
		int																m_id;
		std::function<void(ulong, ulong)> m_action_to_perform;
		ushort														m_frequency;
		ushort														m_phase;
		real															m_priority;

		EXECUTIONMANAGEMENT_API ActionRecord (
																			std::function<void(ulong/*from last execution for this action*/, ulong/*available time*/)> i_action, 
																			ushort i_frequency, 
																			ushort i_phase, 
																			real i_priority);

		EXECUTIONMANAGEMENT_API ActionRecord (ISheduler& i_sheduler,
																					ushort i_frequency,
																					ushort i_phase,
																					real i_priority);

		EXECUTIONMANAGEMENT_API ActionRecord (ushort i_frequency,
																					ushort i_phase,
																					real i_priority);

		};

	template <typename T>
	std::function<void(ulong, ulong)> ConvertMemberFunction (T& i_object, void (T::*i_member_function)(ulong, ulong))
		{
		return std::bind(i_member_function, std::ref(i_object), std::placeholders::_1, std::placeholders::_2);
		}

	EXECUTIONMANAGEMENT_API std::function<void(ulong, ulong)> ConvertShedulerToAction (ISheduler& i_child_sheduler);

	} // SDK

#endif