#include "stdafx.h"

#include <PollingTask.h>

using namespace SDK;
using ::testing::_;
using ::testing::Return;

namespace
	{

	struct PollingTaskStub : SDK::MetaPollingTask<int>
		{
		public:
			PollingTaskStub (SDK::uint i_task_code) : MetaPollingTask(i_task_code){}
			MOCK_METHOD0(UpdateImpl, void());
		};

	struct WorldInterfacing
		{
		MOCK_CONST_METHOD0(GetCurrentTick, SDK::ulong());
		};

	} // namespace

TEST(PollingTaskTests, NoTickGetter_ShouldNotCrash)
	{
	PollingTaskStub task(3);
	EXPECT_EQ(3, task.GetTaskCode());
	EXPECT_TRUE(task.IsStale());
	}

TEST(PollintTaskTests, NoTickGetter_IsStale_ShouldReturnTrueEveryTime)
	{
	PollingTaskStub task(1);

	EXPECT_TRUE(task.IsStale());
	EXPECT_TRUE(task.IsStale());
	}

TEST(PollingTaskTests, NoTickGetter_Update_ShouldCallUpdateImpl)
	{
	PollingTaskStub task(3);
	EXPECT_CALL(task, UpdateImpl()).Times(1);

	task.Update();
	}

TEST(PollingTaskTests, LastUpdateLessThanCurrentTck_IsStale_ShouldReturnTrue)
	{
	PollingTaskStub task(1);
	
	WorldInterfacing world;
	EXPECT_CALL(world, GetCurrentTick()).WillOnce(Return(1));
	task.SetTickGetter(std::bind(&WorldInterfacing::GetCurrentTick, std::ref(world)));

	EXPECT_TRUE(task.IsStale());
	}

TEST(PollingTaskTests, IsStaleAfterUpdate_ShouldReturnFlse)
	{
	PollingTaskStub task(1);

	WorldInterfacing world;
	EXPECT_CALL(world, GetCurrentTick()).WillRepeatedly(Return(1));
	task.SetTickGetter(std::bind(&WorldInterfacing::GetCurrentTick, std::ref(world)));

	EXPECT_TRUE(task.IsStale());
	task.Update();
	EXPECT_FALSE(task.IsStale());
	}

TEST(PollintTaskTests, MetaPollingTask_GetValue_ShouldCallUpdate)
	{
	struct IntPollingTask : MetaPollingTask<int>
		{
		IntPollingTask(uint i_task_code) : MetaPollingTask(i_task_code){}
		MOCK_METHOD0(Update_, void());
		virtual void UpdateImpl() { SetValue(5); Update_(); }
		};
	IntPollingTask task(1);

	task.GetValue();

	WorldInterfacing world;
	EXPECT_CALL(world, GetCurrentTick()).WillRepeatedly(Return(1));
	task.SetTickGetter(std::bind(&WorldInterfacing::GetCurrentTick, std::ref(world)));

	EXPECT_CALL(task, Update_()).Times(1);
	task.GetValue();
	}

TEST(PollingTaskTests, MetaPollingTask_GetValueShouldReturn_5)
	{
	struct IntPollingTask : MetaPollingTask<int>
		{
		IntPollingTask(uint i_task_code) : MetaPollingTask(i_task_code){}
		virtual void UpdateImpl() { SetValue(5); }
		};
	IntPollingTask task(1);
	EXPECT_EQ(5, task.GetValue());
	}

TEST(PollingTaskTests, MetaPollingTask_GetValueShouldReturn_True)
	{
	struct BoolPollingTask : MetaPollingTask<bool>
		{
		BoolPollingTask(uint i_task_code) : MetaPollingTask(i_task_code){}
		virtual void UpdateImpl() { SetValue(true); }
		};
	BoolPollingTask task(1);
	EXPECT_TRUE(task.GetValue());
	}