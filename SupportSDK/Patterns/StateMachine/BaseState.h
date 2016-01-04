#ifndef	__PATTERNS_STATEMACHINE_BASESTATE_H__
#define __PATTERNS_STATEMACHINE_BASESTATE_H__

namespace SDK
{
	template <typename OnUdateParam = float>
	class BaseState
	{
	public:
		virtual ~BaseState() {}

		// stub function - if you want to override one of entry you must include in struct
		//	DEFINE_BASE_FUNCTIONS()
		template <typename EventType>
		void OnEnter(const EventType&) {}

		virtual void OnExit() {}
		virtual void OnUpdate(OnUdateParam i_elapsed_time) {}
	};

	// is needed because of Transition`s code generates OnEnter for all types in TTable with which
	//	 it works (for OnEnter event, and for next_state.OnEnter event)
#define DEFINE_BASE_FUNCTIONS() \
	template <typename EventType> \
	void OnEnter(const EventType&) {}

} // SDK

#endif