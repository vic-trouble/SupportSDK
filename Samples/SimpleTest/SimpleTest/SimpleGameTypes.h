#pragma once

#include <GameCore/Input/InputEvent.h>
#include <GameCore/Input/InputSubscriber.h>


namespace Game
{
	
	class Bullet
	{
	public:
		float m_speed;
		SDK::Vector3 m_position;
		SDK::Vector3 m_direction;

		bool alive;

		Bullet(const SDK::Vector3& i_pos, const SDK::Vector3& i_dir, float i_speed);

		void Update();
		void Draw();
	};


	class StarShip : public SDK::InputSubscriber
	{
	public:
		SDK::Vector3 m_position;
		SDK::Vector3 m_direction;
		SDK::Vector3 direction;
		float m_speed;
		bool m_shooting;

		std::vector<Bullet> bullets;

		StarShip();

		virtual void Update();
		virtual void Draw();
	
		virtual bool KeyPressed(const SDK::KeyEvent& i_evt) override;
		virtual bool KeyReleased(const SDK::KeyEvent& i_evt) override;
	};

} // Game