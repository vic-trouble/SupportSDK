#include "stdafx.h"

#include "EventManagerBaseMocks.h"

#include <Event.h>
#include <EventListener.h>
#include <EventManager.h>

using namespace SDK;
using ::testing::_;


TEST(EventManagerTests, RegisterUnregisterTest)
	{
	EventManagerMock ev_manager;

	EventListenerMock ev_listener(1, &ev_manager);
	ev_manager.RegisterListener(&ev_listener);
	EXPECT_TRUE(ev_manager.ContainsListener(&ev_listener));
	EXPECT_EQ(1, ev_manager.ListenerNumber());

	ev_manager.UnregisterListener(&ev_listener);
	EXPECT_FALSE(ev_manager.ContainsListener(&ev_listener));
	EXPECT_EQ(0, ev_manager.ListenerNumber());
	}

TEST(EventManagerTests, ShedulEventAndDispatch_ShouldCallListeners)
	{
	EventManager ev_manager;
	EventListenerMock ev_listener_0(1, &ev_manager);
	EventListenerMock ev_listener_1(1, &ev_manager);
	EventListenerMock ev_listener_2(0, &ev_manager);

	int test_data = 5;
	EventBase event_base(1, &test_data);
	
	EXPECT_CALL(ev_listener_0, HandleEvent_(TestEvent(event_base))).Times(1);
	EXPECT_CALL(ev_listener_1, HandleEvent_(TestEvent(event_base))).Times(1);
	EXPECT_CALL(ev_listener_2, HandleEvent_(_)).Times(0);

	ev_manager.SheduleEvent(event_base);
	ev_manager.DispatchEvents(1);
	}

#include <Windows.h>

class TimeConsumingEventListener : public SDK::EventListener
	{
	public:
		TimeConsumingEventListener (SDK::uint i_interest_code = 0, SDK::EventManager* ip_manager = nullptr)
			: EventListener(i_interest_code, ip_manager)
			{			}

		MOCK_METHOD1(HandleEvent_, void (TestEvent));

		virtual void HandleEvent(const SDK::EventBase& i_event) override
			{
			HandleEvent_(i_event);
			Sleep(1);
			}
	};

TEST(EventManagerTests, DispatchEvent_3Events_ShouldSkipLastEvent_NotEnoughTime)
	{
	EventManager ev_manager;
	TimeConsumingEventListener first_listener_0(1, &ev_manager);
	TimeConsumingEventListener first_listener_1(1, &ev_manager);

	TimeConsumingEventListener second_listener_0(2, &ev_manager);
	TimeConsumingEventListener second_listener_1(2, &ev_manager);

	TimeConsumingEventListener third_listener_0(3, &ev_manager);
	TimeConsumingEventListener third_listener_1(3, &ev_manager);
	
	EventBase first_ev(1);
	EventBase second_ev(2);
	EventBase third_ev(3);

	EXPECT_CALL(first_listener_0, HandleEvent_(TestEvent(first_ev))).Times(1);
	EXPECT_CALL(first_listener_1, HandleEvent_(TestEvent(first_ev))).Times(1);

	EXPECT_CALL(second_listener_0, HandleEvent_(TestEvent(second_ev))).Times(1);
	EXPECT_CALL(second_listener_1, HandleEvent_(TestEvent(second_ev))).Times(1);

	EXPECT_CALL(third_listener_0, HandleEvent_(TestEvent(third_ev))).Times(0);
	EXPECT_CALL(third_listener_1, HandleEvent_(TestEvent(third_ev))).Times(0);

	ev_manager.SheduleEvent(third_ev);
	ev_manager.SheduleEvent(second_ev);
	ev_manager.SheduleEvent(first_ev);
	
	ev_manager.DispatchEvents(4);
	}
