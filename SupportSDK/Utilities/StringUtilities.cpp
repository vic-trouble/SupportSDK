#include "stdafx.h"
#include "StringUtilities.h"

/////////////////////////////////////////////////////////

StringUtilities::StringUtilities()
{
}

StringUtilities::~StringUtilities()
{
}

bool StringUtilities::AllDigits(const std::string& i_text)
{
	for (size_t i = 0; i < i_text.length(); i++)
	{
		if (!isdigit(i_text[i]))
			return false;
	}

	return true;
}

std::string StringUtilities::ToLower(const std::string& i_value)
{
	std::string valueLower;

	std::for_each(i_value.begin(), i_value.end(), [&valueLower](char c)
	{
		valueLower += tolower(c);
	});
	return valueLower;
}

std::string StringUtilities::GetAsString(std::iostream& i_stream)
{
	int length;
	char* p_buffer;
	
	// get length of file:
	i_stream.seekg (0, std::iostream::end);
	length = static_cast<int>(i_stream.tellg());
	i_stream.seekg (0, std::iostream::beg);

	// allocate memory:
	p_buffer = new char [length];
	//write file
	int pos = 0;
	while(i_stream.good())
	{
		p_buffer[pos] = i_stream.get();
		++pos;
	}

	std::string out(p_buffer, pos);
	delete[] p_buffer;
	return out;
}

bool StringUtilities::ParseBool(const std::string& i_value)
{
  	std::string value_lower = StringUtilities::ToLower(i_value);
	
	if (value_lower .empty() || value_lower  == "false" || value_lower  == "no" || value_lower  == "0")
		return false;
	else
		return true;
}