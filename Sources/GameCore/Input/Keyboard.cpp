#include "stdafx.h"

#include "Keyboard.h"

#include "Platform.h"

namespace SDK
{

	void Keyboard::InitializeTranslator()
	{
		for (int i = 0; i < MAX_KEYS; i++)
			m_translator[i] = Key::UNKNOWN;

#if defined(TARGET_PLATFORM_WIN32)
		m_translator[VK_LEFT] = Key::LEFT;
		m_translator[VK_RIGHT] = Key::RIGHT;
		m_translator[VK_UP] = Key::UP;
		m_translator[VK_DOWN] = Key::DOWN;
		m_translator[VK_ESCAPE] = Key::ESCAPE;
		m_translator[VK_DELETE] = Key::Delete;
		m_translator[VK_BACK] = Key::BACKSPACE;
		m_translator[VK_RETURN] = Key::ENTER;
		m_translator[VK_CONTROL] = Key::CTRL;
		m_translator[VK_MENU] = Key::ALT;
		m_translator[VK_SHIFT] = Key::SHIFT;
		m_translator[VK_CAPITAL] = Key::CAPSLOCK;
		m_translator[VK_SPACE] = Key::SPACE;

		m_translator[VK_F1] = Key::F1;

		// alpha keys
		for (int32 i = 0; i < 26; ++i)
		{
			m_translator[0x41 + i] = Key::A + i;;
		}

		// numeric keys & keys at num pad
		for (int32 i = 0; i < 10; ++i)
		{
			m_translator[0x30 + i] = Key::Key_0 + i;
			m_translator[0x60 + i] = Key::NUMPAD0 + i;
		}
#endif
	}

	Keyboard::Keyboard()
	{
		InitializeTranslator();
		ClearAllKeys();
	}

	bool Keyboard::IsKeyPressed(Key i_code) const
	{
		return m_key_status[i_code];
	}

	void Keyboard::ProcessEvent(const KeyEvent& i_evt)
	{
		if (i_evt.m_key_state == KeyState::Pressed)
			SystemKeyPressed(i_evt.m_key_code);
		else if (i_evt.m_key_state == KeyState::Released)
			SystemKeyReleased(i_evt.m_key_code);
	}

	void Keyboard::SystemKeyPressed(int i_sys_code)
	{
		m_key_status[m_translator[i_sys_code]] = true;
	}

	void Keyboard::SystemKeyReleased(int i_sys_code)
	{
		m_key_status[m_translator[i_sys_code]] = false;
	}

	void Keyboard::ClearAllKeys()
	{
		for (int32 i = 0; i < Key::COUNT; i++)
			m_key_status[i] = false;
	}

} // SDK