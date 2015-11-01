#pragma once

#include <GameCore/Input/InputSubscriber.h>

namespace Game
{
	class InputTest : public SDK::InputSubscriber
	{
	public:
		InputTest();

		SDK::Vector3 m_center;
		int m_speed;

		virtual bool KeyPressed(const SDK::KeyEvent& i_evt);
		virtual bool KeyReleased(const SDK::KeyEvent& i_evt);

		virtual bool MouseMoved(const SDK::MouseEvent& i_evt)
		{
			return false;
		}
		virtual bool MousePressed(const SDK::MouseEvent& i_evt)
		{
			return false;
		}
		virtual bool MouseReleased(const SDK::MouseEvent& i_evt)
		{
			return false;
		}
	};
} // Game