#ifndef __GAMECORE__KEYBOARD_H__
#define __GAMECORE__KEYBOARD_H__

#include "GameCoreAPI.h"

#include "InputEvent.h"

namespace SDK
{
	const static int MAX_KEYS = 256;
	enum Key
	{
		UNKNOWN = 0,
		ESCAPE,
		BACKSPACE,
		TAB,
		ENTER,
		SPACE,

		SHIFT,
		CTRL,
		ALT,

		LWIN,
		RWIN,
		APPS,

		PAUSE,
		CAPSLOCK,
		NUMLOCK,
		SCROLLLOCK,

		PGUP,
		PGDN,
		HOME,
		END,
		INSERT,
		Delete,

		LEFT,
		UP,
		RIGHT,
		DOWN,

		Key_0,
		Key_1,
		Key_2,
		Key_3,
		Key_4,
		Key_5,
		Key_6,
		Key_7,
		Key_8,
		Key_9,

		A,
		B,
		C,
		D,
		E,
		F,
		G,
		H,
		I,
		J,
		K,
		L,
		M,
		N,
		O,
		P,
		Q,
		R,
		S,
		T,
		U,
		V,
		W,
		X,
		Y,
		Z,

		GRAVE,
		MINUS,
		EQUALS,
		BACKSLASH,
		LBRACKET,
		RBRACKET,
		SEMICOLON,
		APOSTROPHE,
		COMMA,
		PERIOD,
		SLASH,

		NUMPAD0,
		NUMPAD1,
		NUMPAD2,
		NUMPAD3,
		NUMPAD4,
		NUMPAD5,
		NUMPAD6,
		NUMPAD7,
		NUMPAD8,
		NUMPAD9,

		MULTIPLY,
		DIVIDE,
		ADD,
		SUBTRACT,
		DECIMAL,

		F1,
		F2,
		F3,
		F4,
		F5,
		F6,
		F7,
		F8,
		F9,
		F10,
		F11,
		F12,

		//Android keys
		BACK,
		MENU,

		COUNT
	};

	class Keyboard
	{
	private:
		int m_translator[MAX_KEYS];
		bool m_key_status[(int)Key::COUNT];//keys pressed for the current frame
		bool m_real_key_status[(int)Key::COUNT];

	protected:
		void InitializeTranslator();

	public:
		Keyboard();

		void ProcessEvent(const KeyEvent& i_evt);

		void SystemKeyPressed(int i_sys_code);
		void SystemKeyReleased(int i_sys_code);

		void ClearAllKeys();

		GAMECORE_EXPORT bool IsKeyPressed(Key i_code) const;
	};

} // SDK

#endif