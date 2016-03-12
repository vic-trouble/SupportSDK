#include "stdafx.h"

#include "PropertyReaders.h"

#include "FileSystem/FileSystem.h"
#include "FileSystem/FileStream.h"

#include <Utilities/HashFunctions.h>
#include <Utilities/lexical_cast.h>

#include "BaseTypesHashes.h"

namespace SDK
{
	////////////////////////////////////////////////////
	// XML

	PropertyElement PropretyReader <(int)ReaderType::XML>::Parse(const std::string& x) const
	{
		return PropertyElement();
	}

	////////////////////////////////////////////////////
	// SDKFormat

	bool IsWhiteSpace(char c)
	{
		return c == ' ' || c == '\t' || c == '\r';
	}	

	ValueType DetermineType(const std::string& i_value, size_t i_offset)
	{
		size_t alpha_char = i_value.find_first_not_of("	", i_offset);
		for (size_t i = i_offset; i < i_value.size(); ++i)
		{
			if (i_value[i] != ' ' && i_value[i] != '	')
			{
				alpha_char = i;
				break;
			}
		}
		const size_t end_line = i_value.find_first_of("\n\0", i_offset + 1);
		if (i_value[alpha_char] == '"')
			return ValueType::String;
		auto val = i_value.substr(i_offset, end_line - i_offset);
		// Property
		if (val.find('{') != std::string::npos)
			return ValueType::PropertyObject;
		// array
		if (val.find('[') != std::string::npos)
			return ValueType::Array;

		// float or double
		if (val.find('.') != std::string::npos)
		{
			if (val.find('f') != std::string::npos)
				return ValueType::Float;
			else
				return ValueType::Double;
		}

		// boolean			
		if (val.find("true") != std::string::npos || val.find("false") != std::string::npos)
			return ValueType::Boolean;

		return ValueType::Int;
	}
	bool IsComment(char c)
	{
		return c == '#';
	}
	bool IsEndl(char c)
	{
		return c == '\n' || c == '\0';
	}
	bool IsSystemChar(char c)
	{
		return IsWhiteSpace(c) || IsComment(c) || IsEndl(c) || c == '[' || c == ']' || c == '{' || c == '}';
	}

	size_t FindFirstNonSystemCharacter(const std::string& value, size_t i_offset)
	{
		for (size_t i = i_offset; i < value.size(); ++i)
		{
			if (!IsSystemChar(value[i]))
				return i;
		}
		return std::string::npos;
	}

	template <typename ValType>
	void AddArray(PropertyElement& o_element, const std::string& i_name, const std::string& value, size_t i_begin, size_t i_end)
	{
		std::vector<ValType> values;
		for (size_t i = i_begin; i <= i_end; ++i)
		{
			if (value[i] == ' ' || value[i] == '\t' || value[i] == ']' || i == i_end)
			{			
				if (i_begin == i_end)
					break;
				ValType v = Utilities::lexical_cast<ValType>(value.substr(i_begin, i - i_begin).c_str());
				values.push_back(v);
				i_begin = value.find_first_not_of(" \t", i + 1);
			}
		}
		o_element.AddValue<std::vector<ValType>>(i_name, values);
	}

	void AddStringArray(PropertyElement& o_element, const std::string& i_name, const std::string& value, size_t i_begin, size_t i_end)
	{
		std::vector<std::string> values;
		// first symbol must be ", so skip it
		++i_begin;
		for (size_t i = i_begin; i <= i_end; ++i)
		{
			if (value[i] == '"' || i == i_end)
			{
				std::string v = value.substr(i_begin, i - i_begin);
				values.push_back(v);
				i_begin = value.find_first_not_of(" \"\t", i + 1);
				i = i_begin;
			}
		}
		o_element.AddValue<std::vector<std::string>>(i_name, values);
	}

	size_t AddArray(PropertyElement& o_element, const std::string& i_name, const std::string& value, size_t i_offset)
	{
		size_t end_val = value.find_first_of("]\n\0", i_offset);
		size_t first_val = value.find_first_not_of(" \t[", i_offset + 1);

		//auto array_str = value.substr(first_val, end_val - first_val);

		ValueType t = DetermineType(value, first_val);
		switch (t)
		{
			case ValueType::Int:
				AddArray<int>(o_element, i_name, value, first_val, end_val);
				break;
			case ValueType::String:
				AddStringArray(o_element, i_name, value, first_val, end_val);
				break;
			case ValueType::Float:
				AddArray<float>(o_element, i_name, value, first_val, end_val);
				break;
		}
		return end_val;
	}

	size_t FindFirstOf(std::string i_search_symbols, const std::string& i_str, size_t i_begin)
	{
		size_t pos = std::string::npos;
		for (char c : i_search_symbols)
		{
			for (size_t i = i_begin; i < i_str.size(); ++i)
			{
				if (i_str[i] == c)
				{
					pos = std::min(pos, i);
					break;
				}
			}
		}
		return pos;
	}

	size_t ParseProperty(PropertyElement& o_element, const std::string& value, size_t i_offset = 0)
	{
		if (value.size() == i_offset)
			return value.size();
		size_t begin = FindFirstNonSystemCharacter(value, i_offset);
		if (begin == std::string::npos)
			return i_offset;

		size_t eq = value.find('=', begin);
		if (eq == std::string::npos)
			return i_offset;

		size_t value_begin = value.find_first_not_of(" \t", eq + 1);

		std::string name = value.substr(begin, eq - begin - 1);
		size_t new_offset = i_offset;
		ValueType t = DetermineType(value, value_begin);
		switch (t)
		{
			case ValueType::PropertyObject:
				{
					PropertyElement new_element;
					new_offset = ParseProperty(new_element, value, value_begin);
					o_element.AddValue<PropertyElement>(name, new_element);
				}
				break;
			case ValueType::Int:
				{
					size_t end_val = FindFirstOf("\n\r\0", value, value_begin);
					new_offset = end_val;
					o_element.AddValue<int>(name, Utilities::lexical_cast<int>(value.substr(value_begin, end_val - value_begin)));
				}
				break;
			case ValueType::Float:
				{
					size_t end_val = FindFirstOf("\n\r\0", value, value_begin);
					new_offset = end_val;
					o_element.AddValue<float>(name, Utilities::lexical_cast<float>(value.substr(value_begin, end_val - value_begin)));
				}
				break;
			case ValueType::Double:
				{
					size_t end_val = FindFirstOf("\n\r\0", value, value_begin);
					new_offset = end_val;
					o_element.AddValue<double>(name, Utilities::lexical_cast<double>(value.substr(value_begin, end_val - value_begin)));
				}
				break;
			case ValueType::Boolean:
				{
					size_t end_val = FindFirstOf("\n\r\0", value, value_begin);
					new_offset = end_val;
					std::string bool_str = value.substr(value_begin, end_val - value_begin);
					o_element.AddValue<bool>(name, Utilities::lexical_cast<bool>(value.substr(value_begin, end_val - value_begin)));
				}
				break;
			case ValueType::String:
				{
					// value_begin points to openning " symbol -> shift
					++value_begin;
					size_t end_val = value.find_first_of("\"", value_begin);
					// end_val points to closing " symbol
					new_offset = end_val + 1;
					o_element.AddValue<std::string>(name, value.substr(value_begin, end_val - value_begin));
				}
				break;
			case ValueType::Array:
				new_offset = AddArray(o_element, name, value, value_begin);
				break;
			default:
				new_offset = value.find_first_of("\n\0", value_begin);
				if (new_offset != std::string::npos)
					++new_offset;
				break;
		}

		// test end object
		for (size_t i = new_offset; i < value.size(); ++i)
		{
			// still that object
			if (!IsSystemChar(value[i]))
				break;
			// end of object
			if (value[i] == '}')
				return i + 1;
		}

		return ParseProperty(o_element, value, new_offset);
	}


	PropertyElement PropretyReader <(int)ReaderType::SDKFormat>::Parse(const std::string& i_file_name) const 
	{
		FS::StreamPtr p_stream(new FS::FileStream(i_file_name));
		if (!p_stream->IsValid())
			return PropertyElement();

		std::istream& io_stream = p_stream->Get();
		return Parse(io_stream);
	}

	PropertyElement PropretyReader <(int)ReaderType::SDKFormat>::Parse(std::istream& io_stream) const
	{
		std::string file_text = FS::ReadFileToString(io_stream);
		PropertyElement root;
		ParseProperty(root, file_text);
		return root;
	}

	////////////////////////////////////////////////////
	// Binary

	PropertyElement PropretyReader <(int)ReaderType::Binary>::Parse(const std::string& x) const 
	{
		return PropertyElement();
	}

} // SDK