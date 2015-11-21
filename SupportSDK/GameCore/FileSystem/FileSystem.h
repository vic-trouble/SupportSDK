#ifndef __GAMECORE_FILESYSTEM_H__
#define __GAMECORE_FILESYSTEM_H__

namespace SDK
{

	namespace FS
	{
		// TODO: make FS :)
		std::string GetApplicationPath();

		std::string ReadFileToString(const std::string& i_path);
		std::string ReadFileToString(std::istream& io_stream);

	} // FS

} // SDK

#endif