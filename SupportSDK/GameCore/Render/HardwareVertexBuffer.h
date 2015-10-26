#ifndef __GAMECORE_HARDWAREVERTEXBUFFER_H__
#define __GAMECORE_HARDWAREVERTEXBUFFER_H__

#include "RenderTypes.h"

namespace SDK
{

	namespace Render
	{

		struct HardwareVertexBuffer
		{
			uint m_size_in_bytes;
			BufferUsageFormat m_usage;
			uint m_hardware_id;
			// number of vertices in array
			uint m_num_vertices;
			uint m_stride;
		};

	}

} // SDK

#endif