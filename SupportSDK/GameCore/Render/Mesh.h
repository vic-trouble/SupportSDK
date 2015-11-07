#ifndef __GAMECORE_MESH_H__
#define __GAMECORE_MESH_H__

#include "../GameCoreAPI.h"

#include "RenderTypes.h"

#include "../Resources/ResourceInformation.h"

namespace SDK
{
	namespace Resources
	{
		class LoadManager;
	}

	namespace Render
	{
		
		/*
		Why mesh is not POD?
		static_assert(<std::is_pod<Mesh>, "Mesh is not POD data type");
		*/

		class Mesh : Resources::Resource
		{
			friend class Resources::LoadManager;
		private:
			VertexBufferHandle	m_vertex_buffer;
			VertexLayoutHandle	m_vertex_layout;

			IndexBufferHandle	m_index_buffer;
			
		private:
			void ClearMesh();

		public:
			GAMECORE_EXPORT Mesh();				
			GAMECORE_EXPORT ~Mesh();

			GAMECORE_EXPORT void Set(VertexBufferHandle i_vertices, IndexBufferHandle i_indices, VertexLayoutHandle i_layout);
			VertexBufferHandle GetVertices() const { return m_vertex_buffer; }
			VertexLayoutHandle GetLayout() const { return m_vertex_layout; }
			IndexBufferHandle GetIndices() const { return m_index_buffer; }
		};

		//static_assert(std::is_pod<Mesh>::value == true, "Mesh is not POD data type");
	}
}

#endif