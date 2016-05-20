#ifndef	__PATTERNS_STATEMACHINE_BASESTATE_H__
#define __PATTERNS_STATEMACHINE_BASESTATE_H__

namespace SDK
{
	template <typename OnUdateParam = float>
	class BaseState
	{
	public:
		virtual ~BaseState() {}

		// if you want to override OnEnter functions than you should define concrete functions
		//	with specific events that your state will handle
		template <typename EventType>
		void OnEnter(const EventType&) {}
		// Is called for first state from SetStates
		void OnEnter(void) {}

		virtual void OnExit() {}
		virtual void OnUpdate(OnUdateParam i_elapsed_time) {}
	};

	template <typename Event, typename StateMachine>
	std::function<void(const Event&)> BindProcessEventFunction(StateMachine& i_fsm)
	{
		static void(StateMachine::*process_func)(const Event&) = &StateMachine::ProcessEvent;
		return std::bind(process_func, std::ref(i_fsm), std::placeholders::_1);
	}

} // SDK

#endif