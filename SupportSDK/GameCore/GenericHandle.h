#ifndef	__GAMECORE_GENERICHANDLE_H__
#define __GAMECORE_GENERICHANDLE_H__

namespace SDK
{

	template<typename T, T N1, T N2, typename Tag, class Enable = void>
	struct GenericHandle;		

	template <typename T, T N1, T N2, typename Tag>
	struct GenericHandle <T, N1, N2, Tag, typename std::enable_if<std::is_integral<T>::value>::type>
	{
		T index : N1;
		T generation : N2;
		//using _HandleType = GenericHandle<T, N1, N2, Tag, std::true_type>;
		inline static GenericHandle InvalidHandle()
		{
			return{ -1, -1 };
		}
	};

	struct InternalHandleTag {};
	using InternalHandle = GenericHandle<int, 12, 20, InternalHandleTag>;

	template <typename T, T N1, T N2, typename Tag>
	bool operator == (const GenericHandle<T, N1, N2, Tag>& left, const GenericHandle<T, N1, N2, Tag>& right)
	{
		return left.index == right.index && left.generation == right.generation;
	}
	template <typename T, T N1, T N2, typename Tag>
	bool operator != (const GenericHandle<T, N1, N2, Tag>& left, const GenericHandle<T, N1, N2, Tag>& right)
	{
		return left.index != right.index || left.generation != right.generation;
	}

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