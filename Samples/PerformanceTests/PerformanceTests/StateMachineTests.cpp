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

	void Test()
	{
		StateMachine<std::unique_ptr<BaseState<>>, 2> fsm(std::unique_ptr<MyState1>(new MyState1), std::unique_ptr<MyState2>(new MyState2));		
		StateMachine<BaseState<>*, 2> fsm1(new MyState1, new MyState2);

		fsm.OnEnter();
		fsm1.OnEnter();

		fsm.SetNext(1);
		fsm.SetNext(2);
		fsm.OnUpdate(1.2f);

		fsm.SetNext<MyState1>();
		fsm1.SetNext<MyState2>();
	}
} // StateMachineTests