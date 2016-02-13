#include "stdafx.h"

#include "Commands.h"

#include "CommandPacket.h"

#include "Core.h"
#include "IRenderer.h"

#include "Render/ShaderSystem.h"
#include "Render/LightsController.h"

namespace SDK
{
	namespace Render
	{
		namespace Commands
		{
			///////////////////////////////////////////////////////////////////////////
			// Draw
			
			void Draw::SetDefaultValues()
			{
				vertices.index = 0;
				layout.index = 0;
				indices.index = 0;
				program.index = -1;
				startVertex = 0;
				vertexCount = 0;
			}

			void Draw::DrawFunction(const void* ip_data)
			{
				auto p_renderer = Core::GetRenderer();
				const Draw* cmd = reinterpret_cast<const Draw*>(ip_data);
				Batch b;
				b.vertices = cmd->vertices;
				b.element = cmd->layout;
				b.indices = cmd->indices;
				
				p_renderer->UseShader(cmd->program);
				p_renderer->Draw(b);
				p_renderer->UseShader({ -1, -1 });
			}

			///////////////////////////////////////////////////////////////////////////
			// Transform

			void Transform::SetDefaultValues()
			{
				m_position[0] = 0.f; m_position[1] = 0.f; m_position[2] = 0.f;
				m_scale[0] = 1.f; m_scale[1] = 1.f; m_scale[2] = 1.f;
				for (size_t i = 0; i < 9; ++i)
					m_rotation[i] = i/3==i%3 ? 1.f : 0.f;
			}

			void Transform::PushTransformation(const void* ip_data)
			{
				auto p_renderer = Core::GetRenderer();
				const Transform* cmd = reinterpret_cast<const Transform*>(ip_data);

				p_renderer->PushMatrix();
				
				Matrix4f transformation;
				transformation.MakeTransform(
						{ cmd->m_position[0], cmd->m_position[1], cmd->m_position[2] }, 
						{ cmd->m_scale[0], cmd->m_scale[1], cmd->m_scale[2] },
						Matrix3(cmd->m_rotation));

				p_renderer->ModifyCurrentMatrix(transformation);
			}

			void Transform::PopTransformation(const void* ip_data)
			{
				auto p_renderer = Core::GetRenderer();
				p_renderer->PopMatrix();
			}

			///////////////////////////////////////////////////////////////////////////
			// SetLight

			void SetLights::ApplyLights(const void* ip_data)
			{
				const SetLights* cmd = reinterpret_cast<const SetLights*>(ip_data);
				auto p_lights = Core::GetRenderer()->GetLightsController();

				cmd->m_was_enabled = p_lights->LightingEnabled();

				if (cmd->m_enabled)
					p_lights->EnableLighting();
				else
					p_lights->DisableLighting();
			}

			void SetLights::RestoreLightConfig(const void* ip_data)
			{
				const SetLights* cmd = reinterpret_cast<const SetLights*>(ip_data);
				auto p_lights = Core::GetRenderer()->GetLightsController();

				if (cmd->m_was_enabled)
					p_lights->EnableLighting();
				else
					p_lights->DisableLighting();
			}

			void SetLights::SetDefaultValues()
			{
				m_enabled = false;
				m_was_enabled = false;
				m_handle = INVALID_LIGHT_HANDLE;
			}

		} // Commands
	} // Render
} // SDK