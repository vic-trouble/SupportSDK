#include "stdafx.h"

#include "StateMachineTests.h"

#include <Patterns/StateMachine.h>

namespace StateMachineTests
{
	using namespace SDK;
	struct MyState1 : public BaseState<>
	{
		virtual void OnEnter()
		{
		}
	};

	struct MyState2 : public BaseState<>
	{
		virtual void OnEnter() override
		{

		}
	};

	struct MyState3 : public BaseState<>
	{
		virtual void OnEnter() override
		{

		}
	};

	struct Event11 : public Event
	{
		Event11(){}
	};

	class MyFSM;
	
	typedef CompoundTransition<
		Transition<MyState1, MyState2, Event11>, 
		CompoundTransition<Transition<MyState2, MyState3, Event11>, Transition<MyState3, MyState1, Event11>>
	> TransitionTable;
	
	class MyFSM : public SDK::StateMachine<MyFSM, SDK::BaseState<>*, 3, TransitionTable>
	{
	public:
	};

	/////////////////////////////


	


	/////////////////////////////


	

	void Test()
	{
		MyFSM my_fsm;
		my_fsm.SetStates(new MyState1, new MyState2, new MyState3);
		my_fsm.SetNext<MyState1>();
		for (size_t i = 0; i < 10; ++i)
		{
			my_fsm.OnUpdate(0.1f);
			std::cout << "Current " << my_fsm.GetCurrent() << std::endl;
			my_fsm.ProcessEvent<Event11>();
		}
	}
} // StateMachineTests