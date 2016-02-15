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
		};

	}

} // SDK

#endif