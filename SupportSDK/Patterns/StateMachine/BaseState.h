#ifndef	__PATTERNS_STATEMACHINE_BASESTATE_H__
#define __PATTERNS_STATEMACHINE_BASESTATE_H__

namespace SDK
{
	
	template <typename OnUdateParam = float>
	class BaseState
	{
	public:
		virtual ~BaseState() {}

		template <typename EventType>
		void OnEnter(const EventType&) {}

		virtual void OnExit() {}
		virtual void OnUpdate(OnUdateParam i_elapsed_time) {}
	};

} // SDK

#endif