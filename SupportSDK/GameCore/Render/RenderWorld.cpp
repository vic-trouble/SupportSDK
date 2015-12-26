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
		// TODO: separate buffers for different jobs
		Commands::CommandBucket<int64> g_ui_buffer;
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
			p_renderer->SetModelViewMatrix(std::move(i_viewport.m_modelview_matrix));
			p_renderer->SetProjectionMatrix(std::move(i_viewport.m_projection_matrix));
			p_renderer->SetProjectionType(i_viewport.m_projection_type);
			
			SetRenderTargets();

			// sort buffers
			gBuffer.Sort();


			// submit buffers
			gBuffer.Submit();			
		}

	} // Render

} // SDK