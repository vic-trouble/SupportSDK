#include "stdafx.h"

#include "FileSystem.h"

#include "FileStream.h"

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

		std::string ReadFileToString(const std::string& i_path)
		{
			FS::FileStream stream(i_path);
			if (!stream.IsValid())
				return std::string();

			return ReadFileToString(stream.Get());
		}

		std::string ReadFileToString(std::istream& io_stream)
		{
			io_stream.seekg(0, io_stream.end);
			auto file_size = io_stream.tellg();
			io_stream.seekg(0, io_stream.beg);			
			std::string result;
			result.resize(file_size);
			io_stream.read(&result[0], file_size);
			return result;
		}

	} // FS

} // SDK