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
		static std::hash<std::string> hash_function;
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

		Render::Mesh LoadManager::LoadObj(const std::string& i_file_name, Render::BufferUsageFormat i_vertices_usage, Render::BufferUsageFormat i_indices_usage)
		{
			Mesh mesh;

			const auto app_path = FS::GetApplicationPath();
			auto path = app_path;
			path.append("\\").append(i_file_name);

			/////////////////
			std::vector<float> vertices;
			std::vector<uint> indices;
			std::string model_name;
			/////////////////

			std::fstream io_stream(path, std::fstream::binary | std::fstream::in);
			assert(io_stream.good());
			Vector3 temp_vec;
			Math::Vector<uint, 3> temp_byte_vec;
			while (io_stream.good())
			{
				char buffer[255];
				io_stream >> buffer;

				switch (buffer[0])
				{
					case 'v':
						if (buffer[1] == '\0')
						{
							io_stream >> temp_vec;
							for (size_t i = 0; i < 3; ++i)
								vertices.push_back(temp_vec[i]);
							break;
						}
						if (buffer[1] == 't')
						{
							io_stream >> temp_vec;
							// TODO
						}
						break;
					case 'o':
						io_stream >> model_name;
						break;
					case 'f':
						if (buffer[1] != '\0')
							break;
						io_stream >> temp_byte_vec;
						for (size_t i = 0; i < 3; ++i)
							indices.push_back(temp_byte_vec[i]);
						break;
					default:
						io_stream.getline(buffer, 254);
						break;
				}
			}
			io_stream.close();

			auto p_mgr = Core::GetRenderer()->GetHardwareBufferMgr();
			mesh.m_vertex_buffer = p_mgr->CreateVertexBuffer(vertices.size(), sizeof(float), BufferUsageFormat::Static, &vertices[0]);
			HardwareIndexBuffer::IndexType ind_type = HardwareIndexBuffer::IndexType::Int;
			/*
			TODO: correct determination of vertex types
			if (indices.size() < 65535)
				ind_type = HardwareIndexBuffer::IndexType::Short;
			else if(indices.size() < 255)
				ind_type = HardwareIndexBuffer::IndexType::Byte;*/		 

			mesh.m_index_buffer = p_mgr->CreateIndexBuffer(ind_type, indices.size(), BufferUsageFormat::Static, &indices[0]);
			mesh.m_vertex_layout = p_mgr->CreateElement(3, Render::VertexSemantic::Position, Render::PrimitiveType::Triangles, Render::ComponentType::Float, false);
			
			return mesh;
		}
		
		Mesh LoadManager::LoadMesh(const std::string& i_file_name, Render::BufferUsageFormat i_vertices_usage, Render::BufferUsageFormat i_indices_usage)
		{
			const size_t id = hash_function(i_file_name);

			auto it = std::find_if(m_loaded_resources.begin(), m_loaded_resources.end(), MeshResourceInfo::FindPredicate(id));
			if (it != m_loaded_resources.end())
			{
				// can return already loaded mesh
				++it->m_use_count;
				return it->m_data;
			}
			
			Mesh mesh = LoadObj(i_file_name, i_vertices_usage, i_indices_usage);
			mesh.m_id = id;
			
			m_loaded_resources.emplace_back(id, mesh);
			MeshResourceInfo* p_info = &m_loaded_resources.back();
			p_info->m_resource_id = id;
			p_info->m_use_count = 1;
			p_info->m_data = mesh;

			return p_info->m_data;
		}


		void LoadManager::ReleaseMesh(const Mesh& i_mesh)
		{
			auto it = std::find_if(m_loaded_resources.begin(), m_loaded_resources.end(), MeshResourceInfo::FindPredicate(i_mesh.m_id));
			// <PANIC> Who creates this? It`s not ours; so we do nothing!
			if (it == m_loaded_resources.end())
			{
				//assert(false && "It is not our Mesh");
				return;
			}

			--it->m_use_count;
			// the last one -> delete buffers
			if (it->m_use_count == 0)
			{

			}
			// otherwise we do nothing
		}
	} // Resources

} // SDK