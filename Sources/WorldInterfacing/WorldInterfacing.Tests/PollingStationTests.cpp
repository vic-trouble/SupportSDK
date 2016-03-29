#include "stdafx.h"

#include <PollingStation.h>

using namespace SDK;

TEST(PollingStationTests, PollRegisteredTask_ShouldReturn_5)
	{
	struct IntPollingTask : MetaPollingTask<int>
		{
		IntPollingTask(uint i_task_code) : MetaPollingTask(i_task_code){}
		virtual void UpdateImpl() override { SetValue(5); }
		};
	
	MetaPollingStation<int> polling_station;
	polling_station.RegisterTask( std::unique_ptr<MetaPollingTask<int>>(new IntPollingTask(1)) );
	EXPECT_EQ(5, polling_station.Poll(1));
	}