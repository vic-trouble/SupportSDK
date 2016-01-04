#include "stdafx.h"

#include "StateMachineTests.h"

#include <Patterns/StateMachine/StateMachine.h>

#include <Utilities/lexical_cast.h>

namespace StateMachineTests
{
	using namespace SDK;
	struct Event11
	{
		std::string name;
		Event11(std::string x) : name(x) {}
		Event11() : name("ev") {}
	};
	struct MyState1 : public BaseState<>
	{
	};
	
	struct MyState2 : public BaseState<>
	{
	};
	

	struct MyState3 : public BaseState<>
	{
		void OnEnter(const Event11& ev)
		{
			std::cout << ev.name << std::endl;
		}
	};

	

	class MyFSM;
	
	typedef CompoundTransition<
		Transition<MyState1, MyState2, Event11>, 
		CompoundTransition<Transition<MyState2, MyState3, Event11>, Transition<MyState3, MyState1, Event11>>
	> TransitionTable;
	class MyFSM : public SDK::StateMachine<MyFSM, SDK::BaseState<>, 3, TransitionTable, SDK::BaseState<>*>
	{
	public:
		typedef CompoundTransition<
			Transition<MyState1, MyState2, Event11>,
			CompoundTransition<Transition<MyState2, MyState3, Event11>, Transition<MyState3, MyState1, Event11>>
		> TransitionTableX;
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
			my_fsm.ProcessEvent<Event11>(Event11(Utilities::lexical_cast(i)));
		}
	}
} // StateMachineTests