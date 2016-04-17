#ifndef __STRINGUTILITIES_H__
#define __STRINGUTILITIES_H__

#include "UtilitiesAPI.h"

#include "noncopyable.h"
#include "lexical_cast.h"

#include <iostream>
#include <sstream>
#include <string>
/////////////////////////////////////////////////////

class StringUtilities : SDK::Utilities::noncopyable
{
private:
  StringUtilities();
  ~StringUtilities();

public:
  static UTILITIES_API bool					AllDigits(const std::string& i_text);
	static UTILITIES_API std::string  ToLower(const std::string& i_value);
  //read all data in stream in string
	static UTILITIES_API std::string	GetAsString(std::iostream& i_stream);
  static UTILITIES_API bool         ParseBool(const std::string& i_value);
  template <typename T>
  static std::string t_to_string(T&& i_value);
};

template <typename T>
std::string StringUtilities::t_to_string(T&& i_value)
{
    std::stringstream s_stream;
    std::string s;
    s_stream << i_value;
    s = s_stream.str();

    return s;
}
namespace SDK
{
	namespace Strings
	{
		template <typename... Args>
		std::string FormatString(const std::string& i_format, Args... i_args)
		{
			const std::string strings[] = { Utilities::lexical_cast(i_args)... };
			std::string buffer;
			buffer.resize(i_format.size() + 50);
			buffer = i_format;
			size_t pos = 0;
			for (size_t i = 0; i < sizeof(strings) / sizeof(std::string); ++i)
			{
				std::string search = "{" + Utilities::lexical_cast(i) + "}";
				pos = buffer.find(search, pos);
				if (pos == std::string::npos)
					continue;
				buffer.replace(pos, search.size(), strings[i]);
				pos += strings[i].size();
			}
			return buffer;
		}
	} // Strings
} // SDK
#endif