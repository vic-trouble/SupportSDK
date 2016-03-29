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
		SDK::uint m_interest_code;
	public:
		EventListenerMock (SDK::uint i_code_interest, SDK::EventManager* ip_manager = nullptr)
			: EventListener(ip_manager)
			, m_interest_code(i_code_interest)
			{			}

		MOCK_METHOD1(HandleEvent_, void (TestEvent));

		virtual void HandleEvent(const SDK::EventBase& i_event) override
			{
			HandleEvent_(TestEvent(i_event));
			}

		virtual bool IsCodeInteresting(const SDK::uint i_code) const override
			{
			return m_interest_code == i_code;
			}
	};

#endif