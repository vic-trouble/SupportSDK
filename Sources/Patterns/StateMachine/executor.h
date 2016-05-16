#ifndef	__PATTERNS_STATEMACHINE_EXECUTOR_H__
#define __PATTERNS_STATEMACHINE_EXECUTOR_H__

#include <Utilities/type_index.h>

namespace SDK
{

	struct ExBase
	{
		ExBase()
		{}
		virtual void Execute() = 0;
	};

	template <typename EventType, typename Handler>
	struct Executor : public ExBase
	{
		EventType m_cached;
		Handler* mp_handler;
		Executor(EventType cached, Handler* ip_handler)
			: m_cached(cached)
			, mp_handler(ip_handler)
		{}
		virtual void Execute() override
		{
			mp_handler->OnEnter(m_cached);
		}
	};

	template <typename Handler>
	struct VoidExecutor : public ExBase
	{
		Handler* mp_handler;
		VoidExecutor(Handler* ip_handler)
			: mp_handler(ip_handler)
		{}
		virtual void Execute() override
		{
			mp_handler->OnEnter();
		}
	};

} // SDK

#endif
