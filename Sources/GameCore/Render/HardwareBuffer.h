#ifndef __GAMECORE_HARDWAREBUFFER_H__
#define __GAMECORE_HARDWAREBUFFER_H__

#include "RenderTypes.h"

namespace SDK
{
	
	namespace Render
	{
		
		class HardwareBuffer
		{
		protected:
			uint m_size_in_bytes;
			BufferUsageFormat m_usage;
			uint m_hardware_id;
			// number of vertices in array
			uint m_num_vertices;
			uint m_stride;

		};

	} // Render

} // SDK

#endif