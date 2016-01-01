#ifndef __GAMECORE_INPUTEVENT_H__
#define __GAMECORE_INPUTEVENT_H__

namespace SDK
{
	enum EventType
	{
		ET_Mouse,
		ET_Keyboard,
		ET_Frame
	};

	struct InputEvent
	{
	protected:
		int m_type;

	public:
		InputEvent(int i_type)
			: m_type(i_type)
		{}
		virtual ~InputEvent(){}

		int GetType() const
		{
			return m_type;
		}
	};

	enum class KeyState
	{
		Pressed,
		Released
	};

	struct KeyEvent : InputEvent
	{
		KeyEvent(int i_key_code, KeyState i_key_state)
			: InputEvent(ET_Keyboard)
			, m_key_code(i_key_code)
			, m_key_state(i_key_state)
		{}

		int m_key_code;
		KeyState m_key_state;
	};

	
	enum MouseButtonID
	{
		Left = 0, 
		Right = 1, 
		Middle = 2,
	};

	enum class MousePhase
	{
		ButtonPressed,
		ButtonReleased,
		Moved
	};

	struct MouseEvent : InputEvent
	{
		MouseEvent(MousePhase i_phase)
			: InputEvent(ET_Mouse)
			, m_phase(i_phase)
			, m_buttons(0)
			, m_x(0.f)
			, m_y(0.f)
		{}

		int m_buttons;
		float m_x;
		float m_y;

		MousePhase m_phase;
	};

}

#endif