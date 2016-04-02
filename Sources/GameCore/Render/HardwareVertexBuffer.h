#ifndef __GAMECORE_HARDWAREVERTEXBUFFER_H__
#define __GAMECORE_HARDWAREVERTEXBUFFER_H__

#include "RenderTypes.h"

#include <cassert>

namespace SDK
{

	namespace Render
	{

		struct HardwareVertexBuffer
		{
			uint m_size_in_bytes;
			BufferUsageFormat m_usage;
			uint m_hardware_id;
			bool m_in_video_mem;

			std::unique_ptr<char> p_dynamic_data;

			HardwareVertexBuffer()
				: p_dynamic_data(nullptr)
				, m_size_in_bytes(0)
				, m_usage(BufferUsageFormat::Static)
				, m_hardware_id(0)
				, m_in_video_mem(false)
			{}

			HardwareVertexBuffer(uint i_size, BufferUsageFormat i_format, uint i_hd_id)
				: p_dynamic_data(nullptr)
				, m_size_in_bytes(i_size)
				, m_usage(i_format)
				, m_hardware_id(i_hd_id)
				, m_in_video_mem(true)
			{}

			HardwareVertexBuffer(uint i_size, BufferUsageFormat i_format)
				: p_dynamic_data(nullptr)
				, m_size_in_bytes(i_size)
				, m_usage(i_format)
				, m_hardware_id(0)
				, m_in_video_mem(false)
			{}

			void SetSubData(const void* ip_data, int i_offset, int i_size)
			{
				if (m_usage == BufferUsageFormat::Static)
				{
					assert(false && "Cannot set for static usage. Try dynamic");
					return;
				}
				if (i_offset + i_size >= m_size_in_bytes || i_offset < 0 || i_size < 0)
				{
					assert(false && "Cannot load more than allocated.");
					return;
				}

				const char* p_data = reinterpret_cast<const char*>(ip_data);
				char* p_buffer = p_dynamic_data.get();
				for (int i = 0; i < i_size; ++i)
					p_buffer[i+i_offset] = p_data[i];
			}

			void LoadToVideoMem()
			{
				m_in_video_mem = true;
			}

			void UnloadFromVideoMem()
			{
				m_in_video_mem = false;
			}
		};

	}

} // SDK

#endif