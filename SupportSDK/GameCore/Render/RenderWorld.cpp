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

		void RenderWorld::Submit(const Viewport& i_viewport)
		{
			auto p_renderer = Core::GetRenderer();

			p_renderer->SetProjectionMatrix(i_viewport.mp_frustum->GetProjectionMatrix());
			p_renderer->SetModelViewMatrix(i_viewport.mp_camera->GetModelViewMatrix());
			SetRenderTargets();

			gBuffer.Sort();
			gBuffer.Submit();			
		}

	} // Render

} // SDK