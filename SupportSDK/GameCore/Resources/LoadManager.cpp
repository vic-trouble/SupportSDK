#include "stdafx.h"
#include "LoadManager.h"

#include "Core.h"
#include "Render/HardwareBufferManagerBase.h"
#include "Render/IRenderer.h"

#include "FileSystem/FileSystem.h"

#include <fstream>
#include <string>

namespace SDK
{
	using namespace Render;

	namespace
	{

		std::istream& operator >> (std::istream& io_stream, Vector3& o_vector)
		{
			//v 4.000000 0.000000 0.000000
			std::string temp;
			getline(io_stream, temp);

			size_t pos = 0;


			for (int i = 0; i < Vector3::VertexNumber; ++i)
			{
				pos = temp.find(' ', pos);
				++pos;
				o_vector[i] = static_cast<float>(atof(temp.substr(pos).c_str()));
			}
			return io_stream;
		}

		std::istream& operator >> (std::istream& io_stream, Math::Vector<SDK::uint, 3>& o_vector)
		{
			//v 4.000000 0.000000 0.000000
			std::string temp;
			getline(io_stream, temp);

			size_t pos = 0;


			for (int i = 0; i < Vector3::VertexNumber; ++i)
			{
				pos = temp.find(' ', pos);
				++pos;
				o_vector[i] = atoi(temp.substr(pos).c_str());
			}
			return io_stream;
		}

		std::istream& operator >> (std::istream& io_stream, Vector4& o_vector)
		{
			//v 4.000000 0.000000 0.000000
			std::string temp;
			getline(io_stream, temp);

			size_t pos = 0;


			for (int i = 0; i < Vector4::VertexNumber; ++i)
			{
				pos = temp.find(' ', pos);
				++pos;
				o_vector[i] = static_cast<float>(atof(temp.substr(pos).c_str()));
			}
			return io_stream;
		}

	}

	namespace Resources
	{
#if defined(CPP_11)
		static std::hash<std::string> hash_func;
		size_t hash_function(const std::string& str)
		{
			return hash_func(str);
		}
#else
		size_t hash_function(const std::string& str)
		{
			size_t hash = 0;
			for (char c : str)
				hash += c;
			return hash;
		}
#endif
		static LoadManager manager;
		LoadManager& g_load_manager = manager;

		FS::StreamPtr LoadManager::OpenStream(const std::string& i_file_name)
		{
			struct FileStream : FS::Stream
			{
				std::fstream m_stream;

				FileStream(const std::string& i_file_name)
				{
					const auto app_path = FS::GetApplicationPath();
					auto path = app_path;
					path.append("\\").append(i_file_name);

					m_stream.open(path, std::fstream::binary | std::fstream::in);
					assert(m_stream.good());
				}
				virtual ~FileStream()
				{
					if (m_stream.is_open())
						m_stream.close();
				}
				virtual std::istream& Get() override
				{
					return m_stream;
				}
				virtual bool IsValid() const override
				{
					return m_stream.is_open() && m_stream.good();
				}
			};
			FS::StreamPtr p_stream(new FileStream(i_file_name));
			if (!p_stream->IsValid())
				return nullptr;
			return p_stream;
		}

	} // Resources

} // SDK