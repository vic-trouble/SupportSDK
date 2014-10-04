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
			struct ActionRecord
				{
				int																m_id;
				std::function<void(ulong, ulong)> m_action_to_perform;
				ushort														m_frequency;
				ushort														m_phase;

				ActionRecord (int i_id, std::function<void(ulong, ulong)> i_action, ushort i_frequency, ushort i_phase);
				};

		private:
			int												m_next_id;
			std::vector<ActionRecord> m_actions;

			ulong											m_current_tick;
			ulong											m_general_time;
			ulong											m_last_time;

		public:
			EXECUTIONMANAGEMENT_API								FrequencySheduler();
			EXECUTIONMANAGEMENT_API virtual				~FrequencySheduler();

			EXECUTIONMANAGEMENT_API virtual int		AddAction (std::function<void(ulong, ulong)> i_action, ushort i_frequency, ushort i_phase) override;
			EXECUTIONMANAGEMENT_API virtual void	RemoveAction (int i_action_id) override;

			EXECUTIONMANAGEMENT_API virtual void	Execute (ulong i_available_time) override;
		};

	} // SDK

#endif