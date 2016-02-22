#ifndef	__GAMECORE_COMMANDS_H__
#define __GAMECORE_COMMANDS_H__

#include "../Render/RenderTypes.h"

namespace SDK
{
	namespace Render
	{
		namespace Commands
		{
			struct Draw
			{
				IMPLEMENT_COMMAND_EXPORT(DrawFunction);

				// COMMAND OWN MEMBERS
				uint vertexCount;
				uint startVertex;

				VertexBufferHandle		vertices;
				VertexLayoutHandle		layout;
				IndexBufferHandle		indices;

				ShaderHandler			program;
			};
			static_assert(std::is_pod<Draw>::value == true, "Draw must be a POD.");

			struct Transform
			{
				IMPLEMENT_COMMAND_WITH_COMPLETION_EXPORT(PushTransformation, PopTransformation);

				enum Actions
				{
					TRANSLATE = 1 << 0,
					SCALE = 1 << 1,
					ROTATE = 1 << 2
				};

				int m_actions;
				float m_position[3];
				float m_scale[3];
				float m_rotation[3 * 3];		

				inline void Translate(Vector3& i_position)
				{
					memcpy(m_position, i_position.GetPointer(), sizeof(float) * 3);
					m_actions |= TRANSLATE;
				}
				inline void Scale(Vector3& i_scale)
				{
					memcpy(m_scale, i_scale.GetPointer(), sizeof(float) * 3);
					m_actions |= SCALE;
				}
				inline void Rotate(Matrix3& i_rotation)
				{
					memcpy(m_rotation, i_rotation[0], sizeof(float) * 3 * 3);
					m_actions |= ROTATE;
				}
			};
			static_assert(std::is_pod<Transform>::value == true, "Transform must be a POD.");

			struct SetLights
			{
				IMPLEMENT_COMMAND_WITH_COMPLETION_EXPORT(ApplyLights, RestoreLightConfig);

				bool m_enabled;
				LightHandle m_handle;

				mutable bool m_was_enabled;
			};
			static_assert(std::is_pod<SetLights>::value == true, "SetLights must be a POD.");

			namespace SetupShaderDetails
			{
				GAMECORE_EXPORT void BindShader(ShaderHandler i_shader, VertexLayoutHandle i_layout);
				GAMECORE_EXPORT void UnbindShader();
			} // SetupShaderDetails
			template <size_t layouts_count>
			struct SetupShader
			{
				static void Bind(const void* ip_data)
				{
					const SetupShader<layouts_count>* cmd = reinterpret_cast<const SetupShader<layouts_count>*>(ip_data);
					SetupShaderDetails::BindShader(cmd->m_shader, cmd->m_layouts[0]);
				}
				static void Unbind(const void*)
				{
					SetupShaderDetails::UnbindShader();
				}
				void SetDefaultValues()
				{
					m_shader.index = -1;
				}
				IMPLEMENT_COMMAND_IMPL(Bind, Unbind);
				
				ShaderHandler m_shader;
				std::array<VertexLayoutHandle, layouts_count> m_layouts;
			};
			
			static_assert(std::is_pod<SetupShader<1>>::value == true, "SetupShader must be a POD.");

		} // Commands
	} // Render
} // SDK

#endif
