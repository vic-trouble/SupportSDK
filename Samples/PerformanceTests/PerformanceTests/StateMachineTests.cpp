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
		DEFINE_BASE_FUNCTIONS();
		void OnEnter()
		{
			//std::cout << "MyState1 Enters first state" << std::endl;
		}
	};
	
	struct MyState2 : public BaseState<>
	{
		DEFINE_BASE_FUNCTIONS();
	};

	struct MyState4 : public BaseState<>
	{
		DEFINE_BASE_FUNCTIONS();
	};
	
	struct MyState3;
	struct Event11;
	typedef CompoundTransition<
		Transition<MyState1, MyState2, Event22>,
		CompoundTransition<Transition<MyState2, MyState3, Event11>, 
		CompoundTransition<Transition<MyState3, MyState4, Event11>, Transition<MyState4, MyState1, Event11>> >
	> TransitionTable;
	class MyFSM : public SDK::StateMachine<MyFSM, SDK::BaseState<>, 4, TransitionTable, MyState1, SDK::BaseState<>*>
	{
	public:
		typedef CompoundTransition<
			Transition<MyState1, MyState2, Event11>,
			CompoundTransition<Transition<MyState2, MyState3, Event11>, Transition<MyState3, MyState1, Event11>>
		> TransitionTableX;

		void OnEnter(const Event22& ev)
		{
			//std::cout << "sdfaerwer" << std::endl;
		}

		void OnEnter()
		{
			//std::cout << "MyFSM Enters first state" << std::endl;
		}

		DEFINE_BASE_FUNCTIONS();

	};
	struct MyState3 : public BaseState<>
	{
		DEFINE_BASE_FUNCTIONS();
		void OnEnter(const Event11& ev)
		{
			//std::cout << ev.name << std::endl;
		}
	};

	using TestTr = TransitionsTable<
		_row<MyState1, MyState2, Event22>,
		_row<MyState2, MyState3, Event11>,
		_row<MyState3, MyState4, Event11>,
		_row<MyState4, MyState1, Event11>
	>;
	class TestFSM : public SDK::StateMachine<TestFSM, SDK::BaseState<>, 4, TestTr, MyState1, SDK::BaseState<>*>
	{};

	/////////////////////////////

	struct M4 : public BaseState<>
	{
		DEFINE_BASE_FUNCTIONS();
		void OnEnter(const Event11& ev)
		{
			//std::cout << "asdasdsd" << std::endl;
		}			
	};
	
	typedef CompoundTransition<
		Transition<MyFSM, M4, Event11>, Transition<M4, MyFSM, Event22>
	> TransTable;

	class MyCompoundFSM : public SDK::StateMachine<MyCompoundFSM, SDK::BaseState<>, 2, TransTable, MyFSM>
	{};

	/////////////////////////////

	template <typename FSM>
	clock_t TestTransitionsPerformance(size_t i_count)
	{
		clock_t beg = clock();
		FSM fsm;
		fsm.SetStates(new MyState1, new MyState2, new MyState3, new MyState4);
		for (size_t i = 0; i < i_count; ++i)
		{
			fsm.OnUpdate(0.1f);			
			fsm.ProcessEvent<Event22>(Event22());
			fsm.ProcessEvent<Event11>(Event11(Utilities::lexical_cast(i)));
			fsm.ProcessEvent<Event11>(Event11(Utilities::lexical_cast(i)));
			fsm.ProcessEvent<Event11>(Event11(Utilities::lexical_cast(i)));
		}
		return clock() - beg;
	}

	void Test()
	{
		MyCompoundFSM x;
		x.SetStates(std::make_unique<MyFSM>(), std::make_unique<M4>());
		x.OnUpdate(.1f);
		x.ProcessEvent(Event11());
		x.OnUpdate(1.f);
		x.ProcessEvent(Event22());
		x.OnUpdate(1.f);
		
		TestFSM test_fsm;
		test_fsm.SetStates(new MyState1, new MyState2, new MyState3, new MyState4);

		MyFSM my_fsm;
		my_fsm.SetStates(new MyState1, new MyState2, new MyState3, new MyState4);
		for (size_t i = 0; i < 10; ++i)
		{
			my_fsm.OnUpdate(0.1f);
			test_fsm.OnUpdate(0.1f);
			//std::cout << "Current " << my_fsm.GetCurrent() << " - " << test_fsm.GetCurrent() << std::endl;
			my_fsm.ProcessEvent<Event22>(Event22());
			my_fsm.ProcessEvent<Event11>(Event11(Utilities::lexical_cast(i)));
			my_fsm.ProcessEvent<Event11>(Event11(Utilities::lexical_cast(i)));
			my_fsm.ProcessEvent<Event11>(Event11(Utilities::lexical_cast(i)));

			test_fsm.ProcessEvent<Event22>(Event22());
			test_fsm.ProcessEvent<Event11>(Event11(Utilities::lexical_cast(i)));
			test_fsm.ProcessEvent<Event11>(Event11(Utilities::lexical_cast(i)));
			test_fsm.ProcessEvent<Event11>(Event11(Utilities::lexical_cast(i)));
		}

		constexpr size_t COUNT = 1000000;
		auto time_fsm = TestTransitionsPerformance<MyFSM>(COUNT);
		auto time_test = TestTransitionsPerformance<TestFSM>(COUNT);
		std::cout << "Time: " << time_fsm << " - " << time_test << std::endl;
	}
} // StateMachineTests