#include "stdafx.h"

#include "SimpleGameTypes.h"

#include <GameCore/Core.h>
#include <GameCore/Render/IRenderer.h>

#include <Math/VectorConstructor.h>

using namespace SDK;

namespace Game
{
	////////////////////////////////////////////////////////
	// Bullet

	Bullet::Bullet(const SDK::Vector3& i_pos, const SDK::Vector3& i_dir, float i_speed)
		: m_position(i_pos)
		, m_direction(i_dir)
		, m_speed(i_speed)
		, alive(true)
	{}

	void Bullet::Update()
	{
		m_position[0] += m_direction[0] * m_speed;
		m_position[1] += m_direction[1] * m_speed;
		if (m_position[1] < 0)
			alive = false;
	}

	void Bullet::Draw()
	{
		auto p_renderer = Core::GetRenderer();
		p_renderer->RenderCircle(m_position, 3., Color(255, 255, 0, 255));
	}


	////////////////////////////////////////////////////////
	// Starship

	StarShip::StarShip()
	{
		m_position = Math::VectorConstructor<float>::Construct(50, 500, 0);
		m_direction = Math::VectorConstructor<float>::Construct(0, -1000, 0);
		m_direction.Normalize();
		m_speed = 10.f;
		m_shooting = false;
	}

	void StarShip::Update()
	{
		m_position += direction;
		for (auto& bullet : bullets)
			bullet.Update();

		bullets.erase(std::find_if(bullets.begin(), bullets.end(), [](Bullet& b){ return b.alive == false; }), bullets.end());
	}

	void StarShip::Draw()
	{
		auto p_renderer = Core::GetRenderer();

		p_renderer->RenderRectangle(m_position, 20, 20, Color(0, 255, 0, 255));
		Vector3 vec = m_position;
		vec[0] = m_position[0] + 20 * m_direction[0];
		vec[1] = m_position[1] + 20 * m_direction[1];
		p_renderer->RenderCircle(vec, 3., Color(0, 255, 0, 255));


		for (auto& bullet : bullets)
			bullet.Draw();
	}

	bool StarShip::KeyPressed(const SDK::KeyEvent& i_evt)
	{
		if (i_evt.m_key_code == VK_LEFT)
			direction[0] = -m_speed;
		else if (i_evt.m_key_code == VK_RIGHT)
			direction[0] = m_speed;
		else if (i_evt.m_key_code == VK_UP)
			direction[1] = -m_speed;
		else if (i_evt.m_key_code == VK_DOWN)
			direction[1] = m_speed;
		else if (i_evt.m_key_code == VK_SPACE)
			bullets.push_back(Bullet(m_position, m_direction, 20));
		return false;
	}

	bool StarShip::KeyReleased(const SDK::KeyEvent& i_evt)
	{
		if (i_evt.m_key_code == VK_LEFT || i_evt.m_key_code == VK_RIGHT)
			direction[0] = 0;
		else if (i_evt.m_key_code == VK_UP || i_evt.m_key_code == VK_DOWN)
			direction[1] = 0;
		return false;
	}

} // Game