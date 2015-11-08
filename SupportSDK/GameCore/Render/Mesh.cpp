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
			: m_valid(false)
		{
		}

		Mesh::Mesh(VertexBufferHandle i_vertices, IndexBufferHandle i_indices, VertexLayoutHandle i_layout)
			: m_vertex_buffer(i_vertices)
			, m_index_buffer(i_indices)
			, m_vertex_layout(i_layout)
			, m_valid(true)
		{		}

		Mesh::~Mesh()
		{		}

	} // Render

} // SDK