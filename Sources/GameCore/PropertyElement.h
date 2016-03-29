#ifndef __GAMECORE_PROPERTYELEMENT_H__
#define __GAMECORE_PROPERTYELEMENT_H__

#include <Utilities/any.h>
#include <Utilities/HashFunctions.h>

#include <vector>

namespace SDK
{
	enum class ValueType
	{
		Boolean,
		Int,
		Float,
		Double,
		String,
		Array,
		PropertyObject
	};

	class PropertyElement
	{
	public:
		template <typename T>
		class iterator;
	private:
		template <typename T>
		friend class iterator;
	private:
		struct ElementVal
		{
			std::string name;
			size_t hash;
			Utilities::any value;

			template <typename ValueType>
			ElementVal(const std::string& i_name, const ValueType& i_value)
				: name(i_name)
				, hash(Utilities::hash_function(i_name))
				, value(i_value)
			{}

			template <typename ValueType>
			ElementVal(const std::string& i_name, ValueType&& i_value)
				: name(i_name)
				, hash(Utilities::hash_function(i_name))
				, value(std::move(i_value))
			{}

			ElementVal() = default;
			ElementVal& operator=(const ElementVal&) = default;
			ElementVal(const ElementVal& other) = default;
		};
		std::vector<ElementVal> m_values;

	public:
		template <typename ValueType>
		size_t AddValue(const std::string& i_name, const ValueType& value)
		{
			m_values.emplace_back(i_name, value);

			return m_values.size() - 1;
		}
	
		template <typename ValueType>
		size_t AddValue(const std::string& i_name, ValueType&& value)
		{
			m_values.emplace_back(i_name, std::move(value));

			return m_values.size() - 1;
		}

		template <typename TargetType>
		const TargetType* GetValuePtr(const std::string& i_name) const
		{
			const size_t hash = Utilities::hash_function(i_name);
			return GetValuePtr<TargetType>(hash);
		}

		template <typename TargetType>
		const TargetType* GetValuePtr(size_t i_hash) const
		{
			auto it = std::find_if(m_values.begin(), m_values.end(), [i_hash](const ElementVal& a)
			{
				return a.hash == i_hash;
			});
			if (it == m_values.end())
				return nullptr;

			return Utilities::any_cast<TargetType>(&it->value);
		}

		const void* GetRawPtr(const std::string& i_name) const
		{
			const size_t hash = Utilities::hash_function(i_name);
			return GetRawPtr(hash);
		}

		const void* GetRawPtr(size_t i_hash) const
		{
			auto it = std::find_if(m_values.begin(), m_values.end(), [i_hash](const ElementVal& a)
			{
				return a.hash == i_hash;
			});
			if (it == m_values.end())
				return nullptr;
			return it->value.get_raw_ptr();
		}

		const Utilities::any* GetAnyPtr(const std::string& i_name) const
		{
			const size_t hash = Utilities::hash_function(i_name);
			return GetAnyPtr(hash);
		}

		const Utilities::any* GetAnyPtr(size_t i_hash) const
		{
			auto it = std::find_if(m_values.begin(), m_values.end(), [i_hash](const ElementVal& a)
			{
				return a.hash == i_hash;
			});
			if (it == m_values.end())
				return nullptr;
			return &it->value;
		}


		template <typename TargetType>
		TargetType GetValue(const std::string& i_name) const
		{
			const TargetType* p_val = GetValuePtr<TargetType>(i_name);

			return p_val != nullptr ? *p_val : TargetType();
		}

		template <typename TargetType>
		TargetType GetValue(size_t i_hash) const
		{
			const TargetType* p_val = GetValuePtr<TargetType>(i_hash);

			return p_val != nullptr ? *p_val : TargetType();
		}

		template <typename TargetType>
		iterator<TargetType> begin() const
		{
			return iterator<TargetType>::begin(*this);
		}
		template <typename TargetType>
		iterator<TargetType> end() const
		{
			return iterator<TargetType>::end(*this);
		}

		template <typename ValueType>
		class iterator
		{
		private:
			const PropertyElement& m_element;
			size_t m_position;
		public:
			iterator(const PropertyElement& i_element)
				: m_element(i_element)
				, m_position(0)
			{
				for (size_t i = m_position; i < m_element.m_values.size(); ++i)
				{
					auto p_value = m_element.m_values[i].value.get_val_ptr<ValueType>();
					if (p_value != nullptr)
					{
						m_position = i;
						break;
					}
				}
			}
			
			std::string element_name() const
			{
				return m_element.m_values[m_position].name;
			}
			
			const ValueType* operator->() const
			{
				return m_element.m_values[m_position].value.get_val_ptr<ValueType>();
			}

			ValueType operator*() const
			{
				return m_element.m_values[m_position].value.get_value<ValueType>();
			}
			iterator& operator++()
			{
				++m_position;

				for (size_t i = m_position; i < m_element.m_values.size(); ++i)
				{
					auto p_value = m_element.m_values[i].value.get_val_ptr<ValueType>();
					if (p_value != nullptr)
					{
						m_position = i;
						break;
					}
				}
				return *this;
			}
			iterator& operator--()
			{
				--m_position;

				for (int i = m_position; i > 0; --i)
				{
					auto p_value = m_element.m_values[i].value.get_val_ptr<ValueType>();
					if (p_value != nullptr)
					{
						m_position = i;
						break;
					}
				}
				return *this;
			}

			bool operator == (const iterator& left) const
			{
				return &left.m_element == &m_element && left.m_position == m_position;
			}
			bool operator != (const iterator& left) const
			{
				return (*this==left) == false;
			}
			static iterator begin(const PropertyElement& i_element)
			{
				iterator it(i_element);
				return it;
			}
			static iterator end(const PropertyElement& i_element)
			{
				iterator it(i_element);
				it.m_position = i_element.m_values.size();
				return it;
			}

		};
	};

} // SDK

#endif
