#pragma once

#include "InputTest.h"
#include "SimpleGameTypes.h"

#include <GameCore/CoreDelegate.h>
#include <GameCore/Render/VertexDeclaration.h>


namespace Game
{
	class CoreDelegateImpl : public SDK::CoreDelegate
	{
		InputTest m_input_subs;

		SDK::Vector3 m_point;
		SDK::QuaternionF m_rotation_curr;
		SDK::QuaternionF m_rotation_src;
		SDK::QuaternionF m_rotation_dest;
		StarShip m_ship;
		float m_time;
		float m_time_to_change;

		SDK::Vector3 vecs[2];
		SDK::Render::Batch batch[2];

		std::vector<SDK::Vector3> points;
		SDK::Vector3 first_point;

		SDK::Vector3 offset;

	private:
		void InitQuaternions();
		void CreateMesh();

		void CreateQuatPoints();

	public:
		virtual ~CoreDelegateImpl(){}

		virtual void OnCreate() override;
		virtual void OnTerminate() override;

		virtual void OnCollapse() override
		{
			std::cout << "Collapse";
		}
		virtual void OnActivate() override
		{
			std::cout << "Activate";
		}

		virtual void Update(float i_elapsed_time) override;
		virtual void Draw() override;
	};
}