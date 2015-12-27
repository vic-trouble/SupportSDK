#include "stdafx.h"

#include "RenderWorld.h"
#include "Frustum.h"
#include "Camera.h"

#include "Core.h"
#include "IRenderer.h"

namespace SDK
{

	namespace Render
	{
		Commands::CommandBucket<int64> gBuffer;
		// shaders?
		void SetRenderTargets()
		{

		}

		// gather buckets for threads
		void MergeBuffers()
		{

		}

		RenderWorld::RenderWorld()
		{			
		}

		void RenderWorld::Submit(Viewport&& i_viewport)
		{
			auto p_renderer = Core::GetRenderer();
			p_renderer->SetProjectionMatrix(std::move(i_viewport.m_projection_matrix));
			p_renderer->SetModelViewMatrix(std::move(i_viewport.m_modelview_matrix));
			p_renderer->SetProjectionType(i_viewport.m_projection_type);
			SetRenderTargets();

			gBuffer.Sort();
			gBuffer.Submit();			
		}

	} // Render

} // SDK