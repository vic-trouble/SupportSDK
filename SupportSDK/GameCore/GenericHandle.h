#ifndef	__GAMECORE_GENERICHANDLE_H__
#define __GAMECORE_GENERICHANDLE_H__

namespace SDK
{

	template <uint N1, uint N2, typename Tag>
	struct GenericHandle
	{
		uint index : N1;
		uint generation : N2;
	};

	template <typename HandlerType, typename BufferType, size_t size = 4096>
	struct GenericBuffersArray
	{
		enum { Size = size };

		uint m_current_index;
		HandlerType m_handlers[size];
		BufferType m_buffer[size];

		GenericBuffersArray()
			: m_current_index(0)
		{
			for (size_t i = 0; i < size; ++i)
			{
				m_handlers[i].index = i;
				m_handlers[i].generation = 0;
				m_buffer[i] = BufferType();
			}
		}
	};

} // SDK

#endif