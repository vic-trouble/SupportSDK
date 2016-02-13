#ifndef __GAMECORE_FILESYSTEM_H__
#define __GAMECORE_FILESYSTEM_H__

#include "../GameCoreAPI.h"

namespace SDK
{

	namespace FS
	{
		// TODO: make FS :)
		GAMECORE_EXPORT std::string GetApplicationPath();

		GAMECORE_EXPORT std::string ReadFileToString(std::istream& io_stream);
		GAMECORE_EXPORT std::string ReadFileToString(const std::string& i_path);

	} // FS

} // SDK

#endif