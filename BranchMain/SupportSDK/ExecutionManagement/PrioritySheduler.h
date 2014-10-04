#ifndef __PRIORITYSHEDULER_H__
#define __PRIORITYSHEDULER_H__

#include "ExecutionManagementAPI.h"

#include "ISheduler.h"
#include "TypeDefines.h"

namespace SDK
	{

	class PrioritySheduler : public ISheduler
		{
		private:

			std::vector<ActionRecord> m_actions;
			int												m_next_index;

		public:
			EXECUTIONMANAGEMENT_API	PrioritySheduler ();
			EXECUTIONMANAGEMENT_API	virtual ~PrioritySheduler ();

			EXECUTIONMANAGEMENT_API virtual int		AddAction (const ActionRecord& i_action) override;
			EXECUTIONMANAGEMENT_API virtual void	RemoveAction (int i_action_id) override;

			EXECUTIONMANAGEMENT_API virtual void	Execute (ulong i_available_time) override;
		};

	} // SDK

#endif