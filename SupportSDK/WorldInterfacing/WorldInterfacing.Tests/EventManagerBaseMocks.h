#ifndef __EVENTMANAGERBASEMOCKS_H__
#define __EVENTMANAGERBASEMOCKS_H__

#include <EventManager.h>
#include <EventListener.h>
#include <Event.h>

class EventManagerMock : public SDK::EventManager
	{
	public:
		bool ContainsListener (SDK::EventListener* ip_listener) const
			{
			auto listeners = GetListeners();
			for (auto p_listener : listeners)
				if (p_listener == ip_listener)
					return true;
			return false;
			}

		size_t ListenerNumber () const
			{
			return GetListeners().size();
			}
	};

struct TestEvent
	{
	SDK::uint				m_code;
	const void*			mp_data;
	TestEvent(const SDK::EventBase& i_event)
		: m_code(i_event.m_code)
		, mp_data(i_event.mp_data)
		{
		}

	bool operator == (const TestEvent& i_other) const
		{
		return this->m_code == i_other.m_code
				&& this->mp_data == i_other.mp_data;
		}
	};

class EventListenerMock : public SDK::EventListener
	{
	public:
		EventListenerMock (SDK::uint i_interest_code = 0, SDK::EventManager* ip_manager = nullptr)
			: EventListener(i_interest_code, ip_manager)
			{			}
		MOCK_METHOD1(HandleEvent_, void (TestEvent));

		virtual void HandleEvent(const SDK::EventBase& i_event) override
			{
			HandleEvent_(TestEvent(i_event));
			}
	};

#endif