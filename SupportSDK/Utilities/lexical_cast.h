#ifndef __UTILITIES_LEXICALCAST_H__
#define __UTILITIES_LEXICALCAST_H__

#include <string>

namespace SDK
{
	namespace Utilities
	{
		namespace impl
		{
			template <typename T>
			struct Converter
			{
				typedef T Type;
				static Type convert_to(const std::string& i_value);
				static std::string convert_from(const Type& i_value);
			};

			template <>
			struct Converter <float>
			{
				static float convert_to(const std::string& i_value)
				{
					return std::stof(i_value.c_str());
				}
				static std::string convert_from(const float& i_value)
				{
					return std::to_string(i_value);
				}
			};

			template <>
			struct Converter <int>
			{
				static int convert_to(const std::string& i_value)
				{
					return std::stoi(i_value.c_str());
				}
				static std::string convert_from(const int& i_value)
				{
					return std::to_string(i_value);
				}
			};

			template <>
			struct Converter <unsigned int>
			{
				static unsigned int convert_to(const std::string& i_value)
				{
					return std::stoi(i_value.c_str());
				}
				static std::string convert_from(const unsigned int& i_value)
				{
					return std::to_string(i_value);
				}
			};

		} // impl

		template <typename T>
		T lexical_cast(const std::string& i_value)
		{
			return impl::Converter<T>::convert_to(i_value);
		}

		template <typename T>
		std::string lexical_cast(T i_val)
		{
			return impl::Converter<T>::convert_from(i_val);
		}

	} // Utilities
} // SDK

#endif