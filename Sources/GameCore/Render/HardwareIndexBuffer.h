#ifndef __GAMECORE_HARDWAREINDEXBUFFER_H__
#define __GAMECORE_HARDWAREINDEXBUFFER_H__

#include "RenderTypes.h"

namespace SDK
{

	namespace Render
	{

		struct HardwareIndexBuffer
		{
			enum IndexType {
				Byte,	// unsigned char
				Short,	// unsigned short
				Int		// unsigned int
			};
			size_t m_size_in_bytes;
			BufferUsageFormat m_usage;
			PrimitiveType m_primitive;
			IndexType m_index_type;
			uint m_hardware_id;
			// number of vertices in array
			uint m_num_indices;
		};

	}

} // SDK

#endif