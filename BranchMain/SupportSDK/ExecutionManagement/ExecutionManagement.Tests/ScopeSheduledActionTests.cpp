#include "stdafx.h"

#include "BaseMocks.h"

#include <ScopeSheduledAction.h>
#include <ISheduler.h>

using namespace SDK;
using ::testing::_;
using ::testing::Return;

const ActionRecordData	TEST_RECORD_DATA = ActionRecordData(1, 0, 0.3f);
const ActionRecord			TEST_ACTION = ActionRecord(1, 0, 0.3f);

TEST(ScopedSheduledActionTests, GeneralTest)
	{
	ShedulerMock sheduler;
	EXPECT_CALL(sheduler, AddAction_(TEST_RECORD_DATA))
		.Times(1)
		.WillOnce(Return(0));

	EXPECT_CALL(sheduler, RemoveAction(0)).Times(1);

	if (true)
		{
		ActionHandlerMock action_handler;
		ScopeSheduledAction scope_action (sheduler, ActionRecord(ConvertMemberFunction<ActionHandlerMock>(action_handler, &ActionHandlerMock::TestMethod), 1, 0, 0.3f) );
		}
	}

TEST(ScopedSheduledActionTests, GeneralTest_MoveConstructor)
	{
	ShedulerMock sheduler;
	EXPECT_CALL(sheduler, AddAction_(TEST_RECORD_DATA))
		.Times(1)
		.WillOnce(Return(0));

	EXPECT_CALL(sheduler, RemoveAction(0)).Times(1);

	if (true)
		{
		ActionHandlerMock action_handler;
		ScopeSheduledAction scope_action = SheduleMemberFunction (sheduler, action_handler, &ActionHandlerMock::TestMethod, TEST_ACTION);
		ScopeSheduledAction scope_action_1 (std::move(scope_action));
		}
	}

TEST(ScopedSheduledActionTests, GeneralTest_FromMethod)
	{
	ShedulerMock sheduler;
	EXPECT_CALL(sheduler, AddAction_(TEST_RECORD_DATA))
		.Times(1)
		.WillOnce(Return(0));

	EXPECT_CALL(sheduler, RemoveAction(0)).Times(1);

	if (true)
		{
		ActionHandlerMock action_handler;
		ScopeSheduledAction scope_action = SheduleMemberFunction (sheduler, action_handler, &ActionHandlerMock::TestMethod, TEST_ACTION);
		}
	}

TEST(ScopedSheduledActionTests, GeneralTest_FromShedule)
	{
	ShedulerMock sheduler;
	EXPECT_CALL(sheduler, AddAction_(TEST_RECORD_DATA))
		.Times(1)
		.WillOnce(Return(0));

	EXPECT_CALL(sheduler, RemoveAction(0)).Times(1);

	if (true)
		{
		ShedulerMock child_sheduler;
		ScopeSheduledAction scope_action = SheduleSheduler (sheduler, child_sheduler, TEST_ACTION);
		}
	}