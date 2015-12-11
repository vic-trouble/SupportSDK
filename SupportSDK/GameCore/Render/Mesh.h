#ifndef __GAMECORE_MESH_H__
#define __GAMECORE_MESH_H__

#include "../GameCoreAPI.h"

#include "RenderTypes.h"

namespace SDK
{
	namespace Render
	{
		class MeshSystem;
		/*
		Why mesh is not POD?
		static_assert(<std::is_pod<Mesh>, "Mesh is not POD data type");
		*/

		class Mesh
		{
		private:
			VertexBufferHandle	m_vertex_buffer;
			VertexLayoutHandle	m_vertex_layout;

			IndexBufferHandle	m_index_buffer;

			bool m_valid;

		public:
			GAMECORE_EXPORT Mesh();
			GAMECORE_EXPORT Mesh(VertexBufferHandle i_vertices, IndexBufferHandle i_indices, VertexLayoutHandle i_layout);
			GAMECORE_EXPORT ~Mesh();
			
			VertexBufferHandle GetVertices() const { return m_vertex_buffer; }
			VertexLayoutHandle GetLayout() const { return m_vertex_layout; }
			IndexBufferHandle GetIndices() const { return m_index_buffer; }

			bool IsValid() const { return m_valid; }
		};

		//static_assert(std::is_pod<Mesh>::value == true, "Mesh is not POD data type");
	}
}

#endif