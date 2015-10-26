#include "stdafx.h"

#include "RenderSystem.h"

#include "Core.h"

#include "IRenderer.h"

namespace SDK
{

	namespace Render
	{

		RenderSystem::RenderSystem()
		{

		}

		void RenderSystem::AddMesh(MeshComponent i_mesh)
		{
			m_meshes.emplace_back(i_mesh);
		}

		void RenderSystem::Update(float i_elapsed_time)
		{

		}

		void RenderSystem::Draw()
		{
			
		}

	} // Render

} // SDK