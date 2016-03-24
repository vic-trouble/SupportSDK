#pragma once

#include "InputTest.h"
#include "SimpleGameTypes.h"

#include <GameCore/CoreDelegate.h>
#include <GameCore/Render/VertexDeclaration.h>

#include <GameCore/Render/Mesh.h>

namespace Game
{
	class CoreDelegateImpl : public SDK::CoreDelegate
	{
		SDK::Render::Batch batch[2];

		SDK::Render::MeshHandler loaded_mesh;
	private:
		void LoadModel();
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