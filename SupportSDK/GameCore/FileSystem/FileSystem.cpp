#include "stdafx.h"

#include "FileSystem.h"

namespace SDK
{

	namespace FS
	{

		std::string GetApplicationPath()
		{
#if defined(TARGET_PLATFORM_WIN32)
			static char buffer[255];
			GetModuleFileNameA(NULL, buffer, sizeof(buffer));
			std::string::size_type pos = std::string(buffer).find_last_of("\\/");
			return std::string(buffer).substr(0, pos);
			return buffer;
#else
			return std::string()
#endif
		}

	} // FS

} // SDK