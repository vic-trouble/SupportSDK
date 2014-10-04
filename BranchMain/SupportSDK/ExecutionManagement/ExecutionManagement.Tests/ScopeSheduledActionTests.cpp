#include "stdafx.h"

#include <ScopeSheduledAction.h>
#include <ISheduler.h>

using namespace SDK;
using ::testing::_;
using ::testing::Return;

class ActionHandlerMock
	{
	public:
		MOCK_METHOD2(TestMethod, void(ulong,ulong));
	};

class ShedulerMock : public ISheduler
	{
	public:
		MOCK_METHOD3(AddAction, int(std::function<void(ulong, ulong)> i_action, ushort i_frequency, ushort i_phase));
		MOCK_METHOD1(RemoveAction, void(int));
		MOCK_METHOD1(Execute, void(ulong));
	};

TEST(ScopedSheduledActionTests, GeneralTest)
	{
	ShedulerMock sheduler;
	EXPECT_CALL(sheduler, AddAction(_,1,0))
		.Times(1)
		.WillOnce(Return(0));

	EXPECT_CALL(sheduler, RemoveAction(0)).Times(1);

	if (true)
		{
		ActionHandlerMock action_handler;
		ScopeSheduledAction scope_action (sheduler, ConvertMemberFunction<ActionHandlerMock>(action_handler, &ActionHandlerMock::TestMethod), 1, 0);
		}
	}

TEST(ScopedSheduledActionTests, GeneralTest_MoveConstructor)
	{
	ShedulerMock sheduler;
	EXPECT_CALL(sheduler, AddAction(_,1,0))
		.Times(1)
		.WillOnce(Return(0));

	EXPECT_CALL(sheduler, RemoveAction(0)).Times(1);

	if (true)
		{
		ActionHandlerMock action_handler;
		ScopeSheduledAction scope_action = SheduleMemberFunction (sheduler, action_handler, &ActionHandlerMock::TestMethod, 1, 0);
		ScopeSheduledAction scope_action_1 (std::move(scope_action));
		}
	}

TEST(ScopedSheduledActionTests, GeneralTest_FromMethod)
	{
	ShedulerMock sheduler;
	EXPECT_CALL(sheduler, AddAction(_,1,0))
		.Times(1)
		.WillOnce(Return(0));

	EXPECT_CALL(sheduler, RemoveAction(0)).Times(1);

	if (true)
		{
		ActionHandlerMock action_handler;
		ScopeSheduledAction scope_action = SheduleMemberFunction (sheduler, action_handler, &ActionHandlerMock::TestMethod, 1, 0);
		}
	}

TEST(ScopedSheduledActionTests, GeneralTest_FromShedule)
	{
	ShedulerMock sheduler;
	EXPECT_CALL(sheduler, AddAction(_,1,0))
		.Times(1)
		.WillOnce(Return(0));

	EXPECT_CALL(sheduler, RemoveAction(0)).Times(1);

	if (true)
		{
		ShedulerMock child_sheduler;
		ScopeSheduledAction scope_action = SheduleSheduler (sheduler, child_sheduler, 1, 0);
		}
	}