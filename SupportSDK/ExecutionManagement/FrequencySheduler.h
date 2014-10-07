#ifndef __FREQUENCYSHEDULER_H__
#define __FREQUENCYSHEDULER_H__

#include "ExecutionManagementAPI.h"

#include "ISheduler.h"
#include "TypeDefines.h"

namespace SDK
	{

	class FrequencySheduler : public ISheduler
		{
		private:
			int												m_next_id;
			std::vector<ActionRecord> m_actions;

			ulong											m_current_tick;

		public:
			EXECUTIONMANAGEMENT_API								FrequencySheduler();
			EXECUTIONMANAGEMENT_API virtual				~FrequencySheduler();

			EXECUTIONMANAGEMENT_API virtual int		AddAction (const ActionRecord& i_action) override;
			EXECUTIONMANAGEMENT_API virtual void	RemoveAction (int i_action_id) override;

			EXECUTIONMANAGEMENT_API virtual void	Execute (ulong i_available_time) override;
		};

	} // SDK

#endif