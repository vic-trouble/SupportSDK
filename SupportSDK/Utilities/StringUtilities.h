#ifndef __STRINGUTILITIES_H__
#define __STRINGUTILITIES_H__

#include "UtilitiesAPI.h"

#include <boost\utility.hpp>

#include <iostream>
#include <sstream>
#include <string>
/////////////////////////////////////////////////////

class StringUtilities : boost::noncopyable
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

#endif