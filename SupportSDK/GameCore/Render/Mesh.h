#ifndef __GAMECORE_MESH_H__
#define __GAMECORE_MESH_H__

#include "../GameCoreAPI.h"

#include "RenderTypes.h"

namespace SDK
{
	namespace Render
	{
		class MeshSystem;
		
		class Mesh
		{
		public:
			struct SubMesh
			{
				VertexBufferHandle	m_vertex_buffer;
				VertexLayoutHandle	m_pos_layout;
				VertexLayoutHandle	m_normal_layout;
				VertexLayoutHandle	m_uv_layout;

				IndexBufferHandle	m_index_buffer;

				SubMesh(VertexBufferHandle i_vertices,
					VertexLayoutHandle i_pos_layout,
					VertexLayoutHandle i_normal_layout,
					VertexLayoutHandle i_uv_layout,
					IndexBufferHandle i_indices)
					: m_vertex_buffer(i_vertices)
					, m_pos_layout(i_pos_layout)
					, m_normal_layout(i_normal_layout)
					, m_uv_layout(i_uv_layout)
					, m_index_buffer(i_indices)
				{}
			};

		private:
			std::vector<SubMesh> m_sub_meshes;

		public:
			GAMECORE_EXPORT void AddSubmesh(VertexBufferHandle i_vertices,
					VertexLayoutHandle i_pos_layout,
					VertexLayoutHandle i_normal_layout,
					VertexLayoutHandle i_uv_layout,
					IndexBufferHandle i_indices);

			size_t GetSubmeshNumber() const { return m_sub_meshes.size(); }
			const SubMesh& GetSubmesh(size_t i) const { return m_sub_meshes[i]; }
			bool IsValid() const { return !m_sub_meshes.empty(); }
		};

	} // Render
} // SDK

#endif