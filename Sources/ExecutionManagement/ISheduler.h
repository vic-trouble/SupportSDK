#ifndef __ISHEDULER_H__
#define __ISHEDULER_H__

#include "ActionRecord.h"
#include "TypeDefines.h"

namespace SDK
	{

	class ISheduler
		{
		public:
			virtual				~ISheduler(){}
			
			virtual int		AddAction (const ActionRecord& i_action) = 0;
			virtual void	RemoveAction (int i_action_id) = 0;

			virtual void	Execute (ulong i_available_time) = 0;
		};

	} // SDK

#endif