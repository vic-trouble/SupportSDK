#ifndef __GAMECORE_MESH_H__
#define __GAMECORE_MESH_H__

#include "RenderTypes.h"

namespace SDK
{
	namespace Render
	{

		struct Mesh
		{
			VertexBufferHandle m_vertex_buffer;
			IndexBufferHandle m_index_buffer;
			
		};

	}
}

#endif