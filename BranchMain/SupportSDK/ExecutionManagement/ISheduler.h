#ifndef __ISHEDULER_H__
#define __ISHEDULER_H__

#include "TypeDefines.h"

namespace SDK
	{

	class ISheduler
		{
		public:
			virtual				~ISheduler(){}
			
			virtual int		AddAction (std::function<void(ulong/*from last execution for this action*/, ulong/*available time*/)> i_action, ushort i_frequency, ushort i_phase) = 0;
			virtual void	RemoveAction (int i_action_id) = 0;

			virtual void	Execute (ulong i_available_time) = 0;
		};

	template <typename T>
	std::function<void(ulong, ulong)> ConvertMemberFunction (T& i_object, void (T::*i_member_function)(ulong, ulong))
		{
		return std::bind(i_member_function, std::ref(i_object), std::placeholders::_1, std::placeholders::_2);
		}

	inline std::function<void(ulong, ulong)> ConvertShedulerToAction (ISheduler& i_child_sheduler)
		{
		return std::bind(&ISheduler::Execute, std::ref(i_child_sheduler), std::placeholders::_2);
		}

	} // SDK

#endif