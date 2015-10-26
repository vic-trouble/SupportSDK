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
			size_t m_size_in_bytes;
			BufferUsageFormat m_usage;

		};

	} // Render

} // SDK

#endif