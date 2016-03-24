#include "stdafx.h"

#include "Mesh.h"

#include "Core.h"
#include "HardwareBufferManagerBase.h"
#include "IRenderer.h"

namespace SDK
{

	namespace Render
	{
		void Mesh::AddSubmesh(VertexBufferHandle i_vertices,
			VertexLayoutHandle i_pos_layout,
			VertexLayoutHandle i_normal_layout,
			VertexLayoutHandle i_uv_layout,
			IndexBufferHandle i_indices)
		{
			m_sub_meshes.emplace_back(i_vertices, i_pos_layout, i_normal_layout, i_uv_layout, i_indices);
		}

	} // Render

} // SDK