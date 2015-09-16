#include "stdafx.h"

#include "InputSystem.h"
#include "InputSubscriber.h"

#if defined(_USRDLL)
	DefineSingletonInstance(SDK::InputSystem)
#endif

namespace SDK
{

	InputSystem::InputSystem()
	{

	}

	InputSystem::~InputSystem()
	{

	}

	void InputSystem::ProcessEvent(const InputEvent& i_evt)
	{
		switch (i_evt.GetType())
		{
			case EventType::ET_Keyboard:
				{
					const KeyEvent& evt = static_cast<const KeyEvent&>(i_evt);
					if (evt.m_key_state == KeyState::Pressed)
					{
						for (auto p_subscriber : m_subscribers)
							p_subscriber->KeyPressed(evt);
					}
					else if (evt.m_key_state == KeyState::Released)
					{
						for (auto p_subscriber : m_subscribers)
							p_subscriber->KeyReleased(evt);
					}
				}
				break;
			case EventType::ET_Mouse:
				{
					const MouseEvent& evt = static_cast<const MouseEvent&>(i_evt);
					if (evt.m_phase == MousePhase::Moved)
					{
						for (auto p_subscriber : m_subscribers)
						{
							if (p_subscriber->MouseMoved(evt))
								break;
						}
					}
					else if (evt.m_phase == MousePhase::ButtonPressed)
					{
						for (auto p_subscriber : m_subscribers)
						{
							if (p_subscriber->MousePressed(evt))
								break;
						}
					}
					else if (evt.m_phase == MousePhase::ButtonReleased)
					{
						for (auto p_subscriber : m_subscribers)
						{
							if (p_subscriber->MouseReleased(evt))
								break;
						}
					}
					
				}
				break;
		}
	}

	void InputSystem::AddSubscriber(InputSubscriber* ip_subscriber)
	{
		m_subscribers.push_back(ip_subscriber);
	}

	void InputSystem::RemoveSubscriber(InputSubscriber* ip_subscriber)
	{
		m_subscribers.erase(std::find(m_subscribers.begin(), m_subscribers.end(), ip_subscriber), m_subscribers.end());
	}

} // SDK