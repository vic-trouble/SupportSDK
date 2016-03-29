#ifndef __GAMECORE_PROPERTYREADERS_H__
#define __GAMECORE_PROPERTYREADERS_H__

#include "PropertyReader.h"

namespace SDK
{

	template <>
	class PropretyReader <(int)ReaderType::XML>
	{
	public:
		PropertyElement Parse(const std::string& i_file_name) const;
		PropertyElement Parse(std::istream& i_stream) const;
	};

	template <>
	class PropretyReader <(int)ReaderType::SDKFormat>
	{
	public:
		PropertyElement Parse(const std::string& i_file_name) const;
		PropertyElement Parse(std::istream& i_stream) const;
	};

	template <>
	class PropretyReader <(int)ReaderType::Binary>
	{
	public:
		PropertyElement Parse(const std::string& i_file_name) const;
		PropertyElement Parse(std::istream& i_stream) const;
	};

} // SDK

#endif
