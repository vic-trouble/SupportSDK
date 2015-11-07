#include "stdafx.h"

#include "Mesh.h"

#include "Resources/LoadManager.h"

#include "Core.h"
#include "HardwareBufferManagerBase.h"
#include "IRenderer.h"

namespace SDK
{

	namespace Render
	{
		// TODO: in initializer list
		Mesh::Mesh()
		{
			m_vertex_buffer.index = 0;
			m_vertex_buffer.generation = 0;

			m_vertex_layout.index = 0;
			m_vertex_layout.generation = 0;

			m_index_buffer.index = 0;
			m_index_buffer.generation = 0;
		}

		Mesh::~Mesh()
		{
			//Resources::g_load_manager.ReleaseMesh(*this);
		}

		void Mesh::Set(VertexBufferHandle i_vertices, IndexBufferHandle i_indices, VertexLayoutHandle i_layout)
		{
			Resources::g_load_manager.ReleaseMesh(*this);
			
			m_vertex_buffer = i_vertices;
			m_vertex_layout = i_layout;
			m_index_buffer = i_indices;
		}

	} // Render

} // SDK