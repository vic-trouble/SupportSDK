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
		void OnEnter()
		{
			//std::cout << "MyState1 Enters first state" << std::endl;
		}
		void OnEnter(const Event11&)
		{

		}
	};
	
	struct MyState2 : public BaseState<>
	{
	};
	class MyFSM;
	struct MyState4 : public BaseState<>
	{
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
		
		void OnEnter(const Event22&)
		{
			std::cout << "asdasd" << std::endl;
		}

		void OnEnter()
		{
			//std::cout << "MyFSM Enters first state" << std::endl;
		}

	};
	struct MyState3 : public BaseState<>
	{
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
	template <typename ParentFSM>
	struct M4 : public BaseState<>
	{
		ParentFSM& m_fsm;
		M4(ParentFSM& i_fsm)
			: m_fsm(i_fsm)
		{}

		void OnEnter(const Event11& ev)
		{
			//std::cout << "asdasdsd" << std::endl;
		}
		virtual void OnUpdate(float dt) override
		{
			m_fsm.ProcessEvent(Event22());
		}
	};
	namespace fsm_in_fsm
	{
		// event that will appear when one of state which is FSM reach its end
		struct int_fsm_break {};

		template <typename FSM, typename Signal>
		struct Finisher : public BaseState<>
		{
			FSM& m_fsm;
			Finisher(FSM& i_fsm) : m_fsm(i_fsm) {}

			virtual void OnUpdate(float) override
			{
				m_fsm.ProcessEvent(Signal());
			}
		};

		struct UpperFSM;
		
		namespace IntFSM
		{
			struct stage_finished {};
			template <typename ParentFSM>
			struct First : public BaseState<>
			{
				ParentFSM& m_fsm;
				First(ParentFSM& i_fsm) : m_fsm(i_fsm) {}
				virtual void OnUpdate(float dt) override
				{
					m_fsm.ProcessEvent(stage_finished());
				}

				void OnEnter()
				{
					std::cout << "Enter first stage" << std::endl;
				}
			};

			template <typename ParentFSM>
			struct Second : public BaseState<>
			{
				ParentFSM& m_fsm;
				Second(ParentFSM& i_fsm) : m_fsm(i_fsm) {}
				virtual void OnUpdate(float dt) override
				{
					m_fsm.ProcessEvent(stage_finished());
				}
				template <typename E>
				void OnEnter(const E& e)
				{
					std::cout << "Enter second stage" << std::endl;
				}
			};

			struct _fsm;
			using Tr = TransitionsTable<
				_row<First<_fsm>, Second<_fsm>, stage_finished>,
				_row<Second<_fsm>, Finisher<UpperFSM, int_fsm_break>, stage_finished>
			>;
			struct _fsm : public SDK::StateMachine<_fsm, SDK::BaseState<>, 3, Tr, First<_fsm>>
			{
				_fsm(UpperFSM& i_fsm)
				{
					SetStates(
						std::make_unique<First<_fsm>>(*this),
						std::make_unique<Second<_fsm>>(*this),
						std::make_unique<Finisher<UpperFSM, int_fsm_break>>(i_fsm)
						);
				}
			};
		} // IntFSM

		struct state_finished {};

		struct TestState : public BaseState<>
		{};
		struct TestState1 : public BaseState<>
		{};

		using Tr = TransitionsTable<
			_row<TestState, IntFSM::_fsm, state_finished>,
			_row<IntFSM::_fsm, TestState1, int_fsm_break>
		>;
		struct UpperFSM : public SDK::StateMachine<UpperFSM, SDK::BaseState<>, 3, Tr, TestState>
		{
			UpperFSM()
			{
				SetStates(
					std::make_unique<TestState>(),
					std::make_unique<TestState1>(),
					std::make_unique<IntFSM::_fsm>(*this)
					);
			}
		};

		void TestInternal()
		{
			UpperFSM fsm;
			fsm.OnUpdate(.1f);

			// go to internal fsm
			fsm.ProcessEvent(state_finished());
			fsm.OnUpdate(.1f);
			// go ot internal::second stage
			fsm.OnUpdate(.1f);
			// finisher
			fsm.OnUpdate(.1f);
			// teststate1
			fsm.OnUpdate(.1f);
		}

	} // fsm_in_fsm
	

	class MyCompoundFSM;
	typedef CompoundTransition<
		Transition<MyFSM, M4<MyCompoundFSM>, Event11>, Transition<M4<MyCompoundFSM>, MyFSM, Event22>
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
		fsm_in_fsm::TestInternal();
		MyCompoundFSM x;
		x.SetStates(std::make_unique<MyFSM>(), std::make_unique<M4<MyCompoundFSM>>(x));
		x.OnUpdate(.1f);
		x.ProcessEvent(Event11());
		x.OnUpdate(1.f);
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