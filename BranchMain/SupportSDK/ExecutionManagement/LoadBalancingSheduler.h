#ifndef __LOADBALANCINGSHEDULER_H__
#define __LOADBALANCINGSHEDULER_H__

#include "ExecutionManagementAPI.h"

#include "ISheduler.h"
#include "TypeDefines.h"

namespace SDK
	{

	class LoadBalancingSheduler : public ISheduler
		{
		private:
			struct ActionRecord
				{
				int																m_id;
				std::function<void(ulong, ulong)> m_action_to_perform;
				ushort														m_frequency;
				ushort														m_phase;
				ulong															m_last_time_run;

				ActionRecord (int i_id, std::function<void(ulong, ulong)> i_action, ushort i_frequency, ushort i_phase);
				};

		private:
			int												m_next_id;
			std::vector<ActionRecord> m_actions;

			ulong											m_current_tick;

		public:
			EXECUTIONMANAGEMENT_API								LoadBalancingSheduler();
			EXECUTIONMANAGEMENT_API virtual				~LoadBalancingSheduler();

			EXECUTIONMANAGEMENT_API virtual int		AddAction (std::function<void(ulong, ulong)> i_action, ushort i_frequency, ushort i_phase) override;
			EXECUTIONMANAGEMENT_API virtual void	RemoveAction (int i_action_id) override;

			EXECUTIONMANAGEMENT_API virtual void	Execute (ulong i_available_time) override;
		};

	} // SDK

#endif