#include "stdafx.h"

#include "BaseMocks.h"

#include <LoadBalancingSheduler.h>
#include <Windows.h>

using namespace SDK;
using ::testing::_;
using ::testing::Ge;


TEST(LoadBalancingShedulerTests, GeneralTest)
	{
	LoadBalancingSheduler sheduler;
	ActionHandlerMock action_handler;
	EXPECT_EQ(0, sheduler.AddAction(ActionRecord(ConvertMemberFunction<ActionHandlerMock>(action_handler, &ActionHandlerMock::TestMethod), 1, 0, 0.3f)) );

	EXPECT_CALL(action_handler, TestMethod(1));

	sheduler.Execute(1);
	}

TEST(LoadBalancingShedulerTests, FirstId_ShouldBe0_Second_1)
	{
	LoadBalancingSheduler sheduler;
	ActionHandlerMock action_handler;
	EXPECT_EQ(0, sheduler.AddAction( ActionRecord(ConvertMemberFunction<ActionHandlerMock>(action_handler, &ActionHandlerMock::TestMethod), 1, 0, 0.3f) ));
	EXPECT_EQ(1, sheduler.AddAction( ActionRecord(ConvertMemberFunction<ActionHandlerMock>(action_handler, &ActionHandlerMock::TestMethod), 1, 0, 0.3f) ));
	}

TEST(LoadBalancingShedulerTests, Frequency2Phase0_10Ticks_ShouldBeCalled_5Times)
	{
	LoadBalancingSheduler sheduler;
	ActionHandlerMock action_handler;
	sheduler.AddAction( ActionRecord(ConvertMemberFunction<ActionHandlerMock>(action_handler, &ActionHandlerMock::TestMethod), 2, 0, 0.3f) );

	EXPECT_CALL(action_handler, TestMethod(1)).Times(5);
	for (size_t i = 0; i < 10; ++i)
		sheduler.Execute(1);
	}

TEST(LoadBalancingShedulerTests, Frequency2Phase1_10Ticks_ShouldBeCalled_4Times)
	{
	LoadBalancingSheduler sheduler;
	ActionHandlerMock action_handler;
	sheduler.AddAction(ActionRecord(ConvertMemberFunction<ActionHandlerMock>(action_handler, &ActionHandlerMock::TestMethod), 2, 1, 0.3f));

	EXPECT_CALL(action_handler, TestMethod(1)).Times(4);
	for (size_t i = 0; i < 10; ++i)
		sheduler.Execute(1);
	}

TEST(LoadBalancingShedulerTests, Frequency2Phase1_RemoveAfter5Tick_ShouldBeCalled_2Times)
	{
	LoadBalancingSheduler sheduler;
	ActionHandlerMock action_handler;
	sheduler.AddAction(ActionRecord(ConvertMemberFunction<ActionHandlerMock>(action_handler, &ActionHandlerMock::TestMethod), 2, 1, 0.3f));

	EXPECT_CALL(action_handler, TestMethod(1)).Times(2);
	for (size_t i = 0; i < 5; ++i)
		sheduler.Execute(1);
	sheduler.RemoveAction(0);
	for (size_t i = 0; i < 5; ++i)
		sheduler.Execute(1);
	}

TEST(LoadBalancingShedulerTests, TimeShouldBe_1)
	{
	LoadBalancingSheduler sheduler;
	ActionHandlerMock action_handler;
	sheduler.AddAction(ActionRecord(ConvertMemberFunction<ActionHandlerMock>(action_handler, &ActionHandlerMock::TestMethod), 2, 1, 0.3f));

	EXPECT_CALL(action_handler, TestMethod(1)).Times(4);
	for (size_t i = 0; i < 10; ++i)
		{
		sheduler.Execute(1);
		Sleep(1);
		}
	}

TEST(LoadBalancingSedulerTests, ForTwoActionsAvailableTimeShouldBeDevided)
	{
	LoadBalancingSheduler sheduler;
	ActionHandlerMock action_handler_0;
	ActionHandlerMock action_handler_1;
	sheduler.AddAction(ActionRecord(ConvertMemberFunction<ActionHandlerMock>(action_handler_0, &ActionHandlerMock::TestMethod), 1, 0, 0.3f));
	sheduler.AddAction(ActionRecord(ConvertMemberFunction<ActionHandlerMock>(action_handler_1, &ActionHandlerMock::TestMethod), 1, 0, 0.3f));

	EXPECT_CALL(action_handler_0, TestMethod(1)).Times(1);
	// this should be 2 because no actions are performed in action_handler_0
	//	but theoretically there are low performance computers where this would be 0
	//	in this case use Ge(1) instead of 2
	EXPECT_CALL(action_handler_1, TestMethod(2)).Times(1);
	//EXPECT_CALL(action_handler_1, TestMethod(_, Ge(1))).Times(1);

	sheduler.Execute(2);
	}

TEST(HierarchicalSedulerTests, GeneralTest)
	{
	LoadBalancingSheduler parent_sheduler;
	ShedulerMock child_sheduler;
	
	parent_sheduler.AddAction( ActionRecord(ConvertShedulerToAction(child_sheduler), 1, 0, 0.3f) );

	EXPECT_CALL (child_sheduler, Execute(3) ).Times(1);

	parent_sheduler.Execute(3);
	}