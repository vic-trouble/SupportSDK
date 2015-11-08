#ifndef __GAMECORE_FILESTREAM_H__
#define __GAMECORE_FILESTREAM_H__

#include "FileSystem/Stream.h"

#include <fstream>

namespace SDK
{

	namespace FS
	{
		class FileStream : public Stream
		{
		public:
			std::fstream m_stream;

			FileStream(const std::string& i_file_name);
			virtual ~FileStream();
			virtual std::istream& Get() override;
			virtual bool IsValid() const override;
		};

	} // FS

} // SDK

#endif