#include "stdafx.h"

#include "InputTest.h"
#include "GameCore/Input/InputEvent.h"

namespace Game
{

	InputTest::InputTest()
	{
		m_center = SDK::Math::VectorConstructor<float>::Construct(0.f, 50.f, 0.f);
		m_speed = 2;
	}


	bool InputTest::KeyPressed(const SDK::KeyEvent& i_evt)
	{
		std::cout << "Key pressed " << i_evt.m_key_code << std::endl;

		if (i_evt.m_key_code == 73/*i*/)
			++m_speed;
		else if (i_evt.m_key_code == 68/*d*/)
			--m_speed;
		else if (i_evt.m_key_code == VK_LEFT)
			m_center[0] -= m_speed;
		else if (i_evt.m_key_code == VK_RIGHT)
			m_center[0] += m_speed;
		else if (i_evt.m_key_code == VK_UP)
			m_center[1] -= m_speed;
		else if (i_evt.m_key_code == VK_DOWN)
			m_center[1] += m_speed;

		return false;
	}

	bool InputTest::KeyReleased(const SDK::KeyEvent& i_evt)
	{
		std::cout << "Key released " << i_evt.m_key_code << std::endl;
		return false;
	}

} // Game