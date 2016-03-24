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

		Mesh::Mesh(VertexBufferHandle i_vertices,
					VertexLayoutHandle i_pos_layout,
					VertexLayoutHandle i_normal_layout,
					VertexLayoutHandle i_uv_layout,
					IndexBufferHandle i_indices)
			: m_vertex_buffer(i_vertices)
			, m_pos_layout(i_pos_layout)
			, m_normal_layout(i_normal_layout)
			, m_uv_layout(i_uv_layout)
			, m_index_buffer(i_indices)			
			, m_valid(true)
		{		}

		Mesh::~Mesh()
		{		}

	} // Render

} // SDK