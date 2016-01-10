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
				IMPLEMENT_COMMAND(DrawFunction);

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
				IMPLEMENT_COMMAND_WITH_COMPLETION(PushTransformation, PopTransformation);

				enum Actions
				{
					TRANSLATE = 1 << 0,
					SCALE = 1 << 1,
					ROTATE = 1 << 2
				};

				int m_actions;
				float m_position[3];
				float m_scale[3];
				float m_matrix[3 * 3];		

				inline void Translate(Vector3& i_position)
				{
					memcpy(m_position, i_position.GetPointer(), sizeof(float) * 3);
					m_actions |= TRANSLATE;
				}
				inline void Scale(Vector3& i_scale)
				{
					memcpy(m_position, i_scale.GetPointer(), sizeof(float) * 3);
					m_actions |= SCALE;
				}
				inline void Rotate(Matrix3& i_rotation)
				{
					memcpy(m_position, i_rotation[0], sizeof(float) * 3 * 3);
					m_actions |= ROTATE;
				}
			};
			static_assert(std::is_pod<Transform>::value == true, "Transform must be a POD.");

			struct SetLights
			{
				IMPLEMENT_COMMAND_WITH_COMPLETION(ApplyLights, RestoreLightConfig);

				bool m_enabled;
				LightHandle m_handle;

				mutable bool m_was_enabled;
			};
			static_assert(std::is_pod<SetLights>::value == true, "SetLights must be a POD.");
		} // Commands
	} // Render
} // SDK

#endif
