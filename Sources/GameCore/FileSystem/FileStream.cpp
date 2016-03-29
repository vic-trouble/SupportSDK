#include "stdafx.h"

#include "FileStream.h"

#include "FileSystem.h"

namespace SDK
{

	namespace FS
	{

		FileStream::FileStream(const std::string& i_file_name)
		{
			const auto app_path = FS::GetApplicationPath();
			auto path = app_path;
			path.append("\\").append(i_file_name);

			m_stream.open(path, std::fstream::binary | std::fstream::in);
			assert(m_stream.good());
		}

		FileStream::~FileStream()
		{
			if (m_stream.is_open())
				m_stream.close();
		}

		std::istream& FileStream::Get()
		{
			return m_stream;
		}

		bool FileStream::IsValid() const
		{
			return m_stream.is_open() && m_stream.good();
		}

	} // FS

} // SDK