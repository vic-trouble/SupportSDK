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
	struct Event22 {};
	struct Event33 {};
	struct MyState1 : public BaseState<>
	{
		
	};
	
	struct MyState2 : public BaseState<>
	{
		DEFINE_BASE_FUNCTIONS();
		void OnEnter(const Event22&)
		{

		}
	};
	
	struct MyState3;
	
	typedef CompoundTransition<
		Transition<MyState1, MyState2, Event22>,
		CompoundTransition<Transition<MyState2, MyState3, Event11>, Transition<MyState3, MyState1, Event11>>
	> TransitionTable;
	class MyFSM : public SDK::StateMachine<MyFSM, SDK::BaseState<>, 3, TransitionTable, SDK::BaseState<>*>
	{
	public:
		typedef CompoundTransition<
			Transition<MyState1, MyState2, Event11>,
			CompoundTransition<Transition<MyState2, MyState3, Event11>, Transition<MyState3, MyState1, Event11>>
		> TransitionTableX;

		void OnEnter(const Event22& ev)
		{
			std::cout << "sdfaerwer" << std::endl;
		}
		DEFINE_BASE_FUNCTIONS();

	};
	struct MyState3 : public BaseState<>
	{
		DEFINE_BASE_FUNCTIONS();
		void OnEnter(const Event11& ev)
		{
			std::cout << ev.name << std::endl;
		}
	};
	/////////////////////////////

	struct M4 : public BaseState<>
	{
		DEFINE_BASE_FUNCTIONS();
		void OnEnter(const Event11& ev)
		{
			std::cout << "asdasdsd" << std::endl;
		}			
	};
	
	typedef CompoundTransition<
		Transition<MyFSM, M4, Event11>, Transition<M4, MyFSM, Event22>
	> TransTable;

	class MyCompoundFSM : public SDK::StateMachine<MyCompoundFSM, SDK::BaseState<>, 2, TransTable, SDK::BaseState<>*>
	{};

	/////////////////////////////


	

	void Test()
	{
		MyCompoundFSM x;
		x.SetStates(new MyFSM, new M4);
		x.SetNext<MyFSM>();
		x.OnUpdate(.1f);
		x.ProcessEvent(Event11());
		x.OnUpdate(1.f);
		x.ProcessEvent(Event22());
		x.OnUpdate(1.f);

		MyFSM my_fsm;
		my_fsm.SetStates(new MyState1, new MyState2, new MyState3);
		my_fsm.SetNext<MyState1>();
		for (size_t i = 0; i < 10; ++i)
		{
			my_fsm.OnUpdate(0.1f);
			std::cout << "Current " << my_fsm.GetCurrent() << std::endl;
			my_fsm.ProcessEvent<Event11>(Event11(Utilities::lexical_cast(i)));
			my_fsm.ProcessEvent<Event22>(Event22());
		}
	}
} // StateMachineTests