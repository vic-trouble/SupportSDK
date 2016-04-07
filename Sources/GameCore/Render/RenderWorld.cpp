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
			p_renderer->SetMatrix(MatrixMode::Projection, i_viewport.m_projection_matrix);
			p_renderer->SetMatrix(MatrixMode::ModelView, i_viewport.m_modelview_matrix);
			p_renderer->SetProjectionType(i_viewport.m_projection_type);
			SetRenderTargets();

			for (auto& bucket : m_command_buckets)
			{
				bucket.Sort();
				bucket.Submit();
			}

			p_renderer->SetMatrix(MatrixMode::Projection, Matrix4f::IDENTITY);
			p_renderer->SetMatrix(MatrixMode::ModelView, Matrix4f::IDENTITY);
			p_renderer->SetProjectionType(Render::ProjectionType::Perspective);
		}

	} // Render

} // SDK