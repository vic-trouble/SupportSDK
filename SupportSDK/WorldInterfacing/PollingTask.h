#ifndef __POLLING_TASK__
#define __POLLING_TASK__

#include "TypeDefines.h"

#include <functional>

namespace SDK
	{

	typedef std::function<ulong()> GetTickNumber;

	template <typename PollingType>
	struct MetaPollingTask
		{
		public:
			typedef PollingType PollingType;

		private:
			uint					m_task_code;
			ulong					m_last_updated;
			GetTickNumber get_tick_number;
			PollingType		m_value;

		private:
			virtual void UpdateImpl() = 0;

		public:
			explicit MetaPollingTask(uint i_task_code) 
				: m_task_code(i_task_code)
				, m_last_updated(0)
				, m_value(0)
				, get_tick_number(nullptr)
				{}
			virtual ~MetaPollingTask() {}

			PollingType GetValue() const
				{
				if (IsStale()) const_cast<MetaPollingTask&>(*this).Update();
				return m_value;
				}
			void				SetValue(const PollingType& i_value) { m_value = i_value; }

			void				SetTickGetter(GetTickNumber i_getter) { get_tick_number = i_getter; }

			bool				IsStale() const 
				{	return get_tick_number == nullptr || m_last_updated < get_tick_number(); }
			uint				GetTaskCode() const { return m_task_code; }

			void				Update()
				{
				UpdateImpl();
				if (get_tick_number)
					m_last_updated = get_tick_number();
				}
		};

	} //SDK

#endif 