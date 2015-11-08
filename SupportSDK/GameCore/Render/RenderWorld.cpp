#include "stdafx.h"

#include "RenderWorld.h"
#include "Frustum.h"
#include "Camera.h"

#include "Core.h"
#include "IRenderer.h"

#include <GL/glew.h>

#include "Render/OpenGL/GlUitlities.h"

namespace SDK
{

	namespace Render
	{
		// shaders?
		void SetRenderTargets()
		{

		}

		RenderWorld::RenderWorld()
		{			
		}

		void RenderWorld::AddCommand(Batch&& i_batch)
		{
			m_buckets.push_back(RenderBucket());
			m_buckets.back().batch = std::move(i_batch);
		}

		void RenderWorld::Submit(const Viewport& i_viewport)
		{
			auto p_renderer = Core::GetRenderer();

			p_renderer->SetProjectionMatrix(i_viewport.mp_frustum->GetProjectionMatrix());
			p_renderer->SetModelViewMatrix(i_viewport.mp_camera->GetModelViewMatrix());
			SetRenderTargets();

			//dispatch commands
			for (auto bucket : m_buckets)
				p_renderer->Draw(bucket.batch);

			m_buckets.clear();
		}

	} // Render

} // SDK