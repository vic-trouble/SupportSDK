#ifndef	__GAMECORE_PROPERTYREADER_H__
#define __GAMECORE_PROPERTYREADER_H__

#include "PropertyElement.h"

namespace SDK
{

	enum class ReaderType
	{
		XML,
		SDKFormat, // JSON like structure {name;type;value}
		Binary // Reads SDKFormat but from binary file {name_hash;type_hash;value}
	};

	template <int RType>
	class PropretyReader
	{
	public:
		PropertyElement Parse(const std::string& i_file_name) const;
		PropertyElement Parse(std::istream& i_stream) const;
	};

} // SDK

#endif