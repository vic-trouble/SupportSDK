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
					return atof(i_value.c_str());
				}
				static std::string convert_from(const float& i_value)
				{
					return "";
				}
			};

			template <>
			struct Converter <int>
			{
				static int convert_to(const std::string& i_value)
				{
					return atoi(i_value.c_str());
				}
				static std::string convert_from(const int& i_value)
				{
					return "";
				}
			};


		} // impl

		template <typename T>
		T lexical_cast(const std::string& i_value)
		{
			return impl::Converter<T>::convert_to(i_value);
		}

	} // Utilities
} // SDK

#endif