#ifndef	__UTILITIES_ANY_H__
#define __UTILITIES_ANY_H__

namespace SDK
{
	namespace Utilities
	{

		class any
		{
		private:
			struct holder_base
			{
				virtual ~holder_base() {}

				virtual const std::type_info& type() const = 0;
				virtual holder_base* clone() const = 0;
			};
			template <typename ValueType>
			struct holder : public holder_base
			{
				ValueType m_value;

				holder(const ValueType& value)
					: m_value(value)
				{}

				holder(ValueType&& value)
					: m_value(std::move(value))
				{}

				virtual const std::type_info& type() const override
				{
					return typeid(ValueType);
				}

				virtual holder_base* clone() const override
				{
					return new holder(m_value);
				}
			};

		private:
			std::unique_ptr<holder_base> mp_value;

		public:

			template <typename ValueType>
			any(const ValueType& value)
				: mp_value(new holder<ValueType>(value))
			{}

			template <typename ValueType>
			any(ValueType&& value)
				: mp_value(new holder<ValueType>(value))
			{}

			any(const any& other)
				: mp_value(other.mp_value ? other.mp_value->clone() : nullptr)
			{}

			// Move constructor
			any(any&& other)
				: mp_value(std::move(other.mp_value))
			{
				other.mp_value = nullptr;
			}

			template <typename TargetType>
			TargetType get_value() const
			{
				const TargetType* p_val = get_val_ptr<TargetType>();

				return p_val != nullptr ? *p_val : TargetType();
			}

			template <typename TargetType>
			const TargetType* get_val_ptr() const
			{
				return mp_value->type() == typeid(TargetType)
					? &static_cast<const holder<TargetType>*>(mp_value.get())->m_value
					: nullptr;
			}
		};

		template <typename TargetType>
		TargetType any_cast(const any& i_any)
		{
			return i_any.get_value<TargetType>();
		}

		template <typename TargetType>
		const TargetType* any_cast(const any* ip_any)
		{
			return ip_any != nullptr ? ip_any->get_val_ptr<TargetType>() : nullptr;
		}

	} // Utilities
} // SDK

#endif
