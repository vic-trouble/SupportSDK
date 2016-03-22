#include "stdafx.h"

#include "Mesh.h"

#include "Core.h"
#include "HardwareBufferManagerBase.h"
#include "IRenderer.h"

namespace SDK
{

	namespace Render
	{
		// TODO: in initializer list
		Mesh::Mesh()
			: m_valid(false)
		{
		}

		Mesh::Mesh(VertexBufferHandle i_vertices, VertexBufferHandle i_uvs, IndexBufferHandle i_indices, VertexLayoutHandle i_layout, VertexLayoutHandle i_uv_layout)
			: m_vertex_buffer(i_vertices)
			, m_uvs(i_uvs)
			, m_index_buffer(i_indices)
			, m_vertex_layout(i_layout)
			, m_uv_layout(i_uv_layout)
			, m_valid(true)
		{		}

		Mesh::~Mesh()
		{		}

	} // Render

} // SDK