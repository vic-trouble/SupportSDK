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
			m_buckets.push_back(RenderBucket());
		}

		void RenderWorld::AddCommand(Batch&& i_batch)
		{
			m_buckets[0].batch = i_batch;
		}

		void RenderWorld::Submit(const Viewport& i_viewport)
		{
			auto p_renderer = Core::GetRenderer();

			p_renderer->SetProjectionMatrix(i_viewport.mp_frustum->GetProjectionMatrix());
			p_renderer->SetModelViewMatrix(i_viewport.mp_camera->GetModelViewMatrix());
			SetRenderTargets();

			//dispatch commands
			/*for (auto s : m_buckets)
				p_renderer->Draw(s.batch);			*/
		}

	} // Render

} // SDK