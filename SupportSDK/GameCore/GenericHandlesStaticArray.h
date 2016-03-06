#ifndef	__GAMECORE_GENERICHANDLESSTATICARRAY_H__
#define __GAMECORE_GENERICHANDLESSTATICARRAY_H__

#include "GenericHandle.h"

#include <vector>

namespace SDK
{

	namespace detail
	{
		template <typename DataType, typename BaseType, class Enable = void>
		struct TypeTraits;

		template <typename DataType, typename BaseType>
		struct TypeTraits <DataType, BaseType,
			typename std::enable_if< std::is_same<std::unique_ptr<BaseType>, DataType>::value >::type >
		{
			template <typename ElementType, typename... Args>
			static DataType CreateImpl(Args... args)
			{
				return std::make_unique<ElementType>(args...);
			}

			static void Destroy(DataType& o_data)
			{
				o_data.reset();
			}

			static BaseType* Access(DataType& i_data)
			{
				return i_data.get();
			}

			static const BaseType* Access(const DataType& i_data)
			{
				return i_data.get();
			}
		};

		template <typename typename DataType, typename BaseType>
		struct TypeTraits <DataType, BaseType,
			typename std::enable_if<!std::is_same<std::unique_ptr<BaseType>, DataType>::value>::type>
		{
			template <typename ElementType, typename... Args>
			static DataType CreateImpl(Args... args)
			{
				return DataType(args...);
			}

			static void Destroy(DataType& o_data)
			{
				o_data = BaseType();
			}

			static BaseType* Access(DataType& i_data)
			{
				return &i_data;
			}

			static const BaseType* Access(const DataType& i_data)
			{
				return &i_data;
			}
		};

		template <typename T>
		struct DetermineBase
		{
			typedef T BaseType;
		};

		template <typename T>
		struct DetermineBase <std::unique_ptr<T>>
		{
			typedef T BaseType;
		};
	}

	template <typename HandleType, typename DataType, size_t size>
	class GenericHandlesStaticArray
	{
	public:
		enum { Size = size };
		using HType = HandleType;
		using DType = DataType;
	public:
		using ArrayElement = std::pair<HandleType, DataType>;
		std::array<ArrayElement, Size> m_elements;
		int m_max_element;

	public:
		GenericHandlesStaticArray()
			: m_max_element(0)
		{
			ClearAll();
		}

		template <typename... Args>
		HType Create(Args... args)
		{
			// find appropriate element
			int index = -1;
			if (m_max_element < Size)
				index = m_max_element + 1;
			else
			{
				// first empty element
				for (size_t i = 0; i < Size; ++i)
				{
					if (m_elements[i].first.index == -1)
					{
						index = i;
						break;
					}
				}
			}
			// no empty slots
			if (index == -1)
			{
				assert(false && "Buffer is overflowed");
				return HType::InvalidHandle();
			}


			auto& element = m_elements[index];
			element.first.index = index;
			++element.first.generation;

			element.second = DType(args...);
			m_max_element = std::max(index, m_max_element);

			return element.first;
		}		

		void Destroy(HandleType i_handle)
		{
			if (!IsValid(i_handle))
				return;
			
			m_elements[i_handle.index] = DType();
			m_elements[i_handle.index].first.index = -1;
		}

		DType* Access(HandleType i_handle)
		{
			if (!IsValid(i_handle))
				return nullptr;

			return &m_elements[i_handle.index].second;
		}

		const DType* Access(HandleType i_handle) const
		{
			if (!IsValid(i_handle))
				return nullptr;

			return &m_elements[i_handle.index].second;
		}

		bool IsValid(HandleType i_handle) const
		{
			if (i_handle.index == -1 || static_cast<int>(Size) <= i_handle.index)
				return false;
			if (m_elements[i_handle.index].first.index != i_handle.index || m_elements[i_handle.index].first.generation != i_handle.generation)
				return false;

			return true;
		}

		void ClearAll()
		{
			for (auto& element : m_elements)
			{
				element.first = HType::InvalidHandle();
				element.second = DType();
			}
		}
	};

}

#endif