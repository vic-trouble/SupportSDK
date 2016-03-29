#include "stdafx.h"

#include "BaseMocks.h"

#include <FrequencySheduler.h>

#include <../Utilities/TimeUtilities.h>

using namespace SDK;
using ::testing::_;

TEST(FrequencyShedulerTests, GeneralTest)
	{
	FrequencySheduler sheduler;
	ActionHandlerMock action_handler;
	EXPECT_EQ(0, sheduler.AddAction( ActionRecord(ConvertMemberFunction<ActionHandlerMock>(action_handler, &ActionHandlerMock::TestMethod), 1, 0, 0.3f) ));

	EXPECT_CALL(action_handler, TestMethod(1));

	sheduler.Execute(1);
	}

TEST(FrequencyShedulerTests, FirstId_ShouldBe0_Second_1)
	{
	FrequencySheduler sheduler;
	ActionHandlerMock action_handler;
	EXPECT_EQ(0, sheduler.AddAction( ActionRecord(ConvertMemberFunction<ActionHandlerMock>(action_handler, &ActionHandlerMock::TestMethod), 1, 0, 0.3f) ));
	EXPECT_EQ(1, sheduler.AddAction( ActionRecord(ConvertMemberFunction<ActionHandlerMock>(action_handler, &ActionHandlerMock::TestMethod), 1, 0, 0.3f) ));
	}

TEST(FrequencyShedulerTests, Frequency2Phase0_10Ticks_ShouldBeCalled_5Times)
	{
	FrequencySheduler sheduler;
	ActionHandlerMock action_handler;
	sheduler.AddAction( ActionRecord(ConvertMemberFunction<ActionHandlerMock>(action_handler, &ActionHandlerMock::TestMethod), 2, 0, 0.3f) );

	EXPECT_CALL(action_handler, TestMethod(1)).Times(5);
	for (size_t i = 0; i < 10; ++i)
		sheduler.Execute(1);
	}

TEST(FrequencyShedulerTests, Frequency2Phase1_10Ticks_ShouldBeCalled_4Times)
	{
	FrequencySheduler sheduler;
	ActionHandlerMock action_handler;
	sheduler.AddAction( ActionRecord(ConvertMemberFunction<ActionHandlerMock>(action_handler, &ActionHandlerMock::TestMethod), 2, 1, 0.3f) );

	EXPECT_CALL(action_handler, TestMethod(1)).Times(4);
	for (size_t i = 0; i < 10; ++i)
		sheduler.Execute(1);
	}

TEST(FrequencyShedulerTests, Frequency2Phase1_RemoveAfter5Tick_ShouldBeCalled_2Times)
	{
	FrequencySheduler sheduler;
	ActionHandlerMock action_handler;
	sheduler.AddAction( ActionRecord(ConvertMemberFunction<ActionHandlerMock>(action_handler, &ActionHandlerMock::TestMethod), 2, 1, 0.3f) );

	EXPECT_CALL(action_handler, TestMethod(1)).Times(2);
	for (size_t i = 0; i < 5; ++i)
		sheduler.Execute(1);
	sheduler.RemoveAction(0);
	for (size_t i = 0; i < 5; ++i)
		sheduler.Execute(1);
	}

TEST(FrequencyShedulerTests, TimeShouldBe_1)
	{
	FrequencySheduler sheduler;
	ActionHandlerMock action_handler;
	sheduler.AddAction( ActionRecord(ConvertMemberFunction<ActionHandlerMock>(action_handler, &ActionHandlerMock::TestMethod), 2, 1, 0.3f) );

	EXPECT_CALL(action_handler, TestMethod(1)).Times(4);
	for (size_t i = 0; i < 10; ++i)
		{
		sheduler.Execute(1);
		Utilities::SleepMiliseconds(1);
		}
	}