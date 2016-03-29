#include "stdafx.h"

#include <Event.h>
#include <EventManager.h>
#include <EventListenerBroadCasting.h>

using namespace SDK;

namespace
	{

	class BroadCastListenerrMock : public EventListenerBroadCasting
	{
	public:
		virtual void HandleEvent(const SDK::EventBase& i_event) override
			{			}
	};

	} // namespace

TEST(EventListenerBroadCastingTests, GeneralTest_ShouldTrueForAddedCodes)
	{
	BroadCastListenerrMock listener;
	listener.AddCode(0);
	listener.AddCode(10);
	listener.AddCode(13);
	listener.Initialize();

	EXPECT_TRUE(listener.IsCodeInteresting(0));
	EXPECT_TRUE(listener.IsCodeInteresting(10));
	EXPECT_TRUE(listener.IsCodeInteresting(13));
	}

TEST(EventListenerBroadCastingTests, Flush_ShouldFalse)
	{
	BroadCastListenerrMock listener;
	listener.AddCode(0);
	listener.AddCode(10);
	listener.AddCode(13);
	listener.Initialize();

	listener.Flush();
	EXPECT_FALSE(listener.IsCodeInteresting(0));
	EXPECT_FALSE(listener.IsCodeInteresting(10));
	EXPECT_FALSE(listener.IsCodeInteresting(13));
	}