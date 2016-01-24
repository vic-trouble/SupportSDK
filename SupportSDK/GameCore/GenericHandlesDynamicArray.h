#ifndef	__GAMECORE_GENERICHANDLESDYNAMICARRAY_H__
#define __GAMECORE_GENERICHANDLESDYNAMICARRAY_H__

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

	template <typename HandleType, typename DataType, typename BaseType = detail::DetermineBase<DataType>::BaseType>
	class GenericHandleDynamicArray
	{
	public:
		using ArrayElement = std::pair<HandleType, DataType>;
		using TypeTraits = detail::TypeTraits<DataType, BaseType>;
		std::vector<ArrayElement> m_elements;

	public:		
		template <typename ElementType = BaseType, typename... Args>
		HandleType CreateNew(Args... args)
		{
			// Create control
			DataType obj = TypeTraits::CreateImpl<ElementType>(args...);

			HandleType handle{ static_cast<int>(m_elements.size()), 0 };
			// find free control
			{
				for (size_t i = 0; i < m_elements.size(); ++i)
				{
					auto& element = m_elements[i];
					if (element.first.index == -1)
					{
						handle = { static_cast<int>(i), element.first.generation + 1 };
						break;
					}
				}
			}

			if (handle.index == m_elements.size())
				m_elements.push_back(std::make_pair(handle, std::move(obj)));
			else
			{
				m_elements[handle.index].first = handle;
				m_elements[handle.index].second = std::move(obj);
			}
			return handle;
		}

		void Destroy(HandleType i_handle)
		{
			if (!IsValid(i_handle))
				return;

			TypeTraits::Destroy(m_elements[i_handle.index].second);
			m_elements[i_handle.index].first.index = -1;
		}

		BaseType* Access(HandleType i_handle)
		{
			if (!IsValid(i_handle))
				return nullptr;

			return TypeTraits::Access(m_elements[i_handle.index].second);
		}

		const BaseType* Access(HandleType i_handle) const
		{
			if (!IsValid(i_handle))
				return nullptr;

			return TypeTraits::Access(m_elements[i_handle.index].second);
		}

		bool IsValid(HandleType i_handle) const
		{
			if (i_handle.index == -1 || static_cast<int>(m_elements.size()) <= i_handle.index)
				return false;
			if (m_elements[i_handle.index].first.index != i_handle.index || m_elements[i_handle.index].first.generation != i_handle.generation)
				return false;

			return true;
		}

		void ClearAll()
		{
			m_elements.clear();
		}
	};

}

#endif