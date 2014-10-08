#include "stdafx.h"

#include "EventManagerBaseMocks.h"

#include <Event.h>
#include <EventListener.h>
#include <EventManager.h>

using namespace SDK;
using ::testing::_;

TEST(EventListenerTests, DefaultsTest_ShouldNotCrash)
	{
	EventListenerMock ev_listener;

	EXPECT_TRUE(ev_listener.IsCodeInteresting(0));

	ev_listener.SetInterestCode(1);
	EXPECT_TRUE(ev_listener.IsCodeInteresting(1));
	EXPECT_FALSE(ev_listener.IsCodeInteresting(0));
	}

TEST(EventListenerTests, ManagerNotNull_ShouldRegisterAndUnregister)
	{
	EventManagerMock ev_manager;

	if (true)
		{
		EventListenerMock ev_listener(1, &ev_manager);
		EXPECT_TRUE(ev_manager.ContainsListener(&ev_listener));
		EXPECT_EQ(1, ev_manager.ListenerNumber());
		}
	EXPECT_EQ(0, ev_manager.ListenerNumber());
	}