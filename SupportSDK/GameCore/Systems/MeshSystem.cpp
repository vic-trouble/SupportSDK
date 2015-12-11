#include "stdafx.h"

#include "MeshSystem.h"

#include "Core.h"
#include "Render/Commands.h"
#include "Render/HardwareBufferManagerBase.h"
#include "Render/IRenderer.h"
#include "Applications/ApplicationBase.h"
#include "Render/RenderWorld.h"
#include "Resources/ResourceManager.h"

#include "TransformationsSystem.h"
#include "EntityManager.h"

#include <string>

namespace SDK
{
	struct MeshInformation
	{
		typedef Render::Mesh ResultType;

		Render::BufferUsageFormat m_ver_usage;
		Render::BufferUsageFormat m_ind_usage;
	};

	namespace Resources
	{	
		namespace Serialization
		{

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

			template <>
			struct Definition <Render::Mesh>
			{
				typedef MeshInformation InfoType;
			};

			template <>
			struct LoaderImpl < Render::Mesh >
			{
				static std::pair<LoadResult, Render::Mesh> Load(std::istream& io_stream, MeshInformation i_info)
				{
					using namespace Render;

					/////////////////
					std::vector<float> vertices;
					std::vector<uint> indices;
					std::string model_name;
					/////////////////

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
					
					auto p_mgr = Core::GetRenderer()->GetHardwareBufferMgr();
					auto ver_buf = p_mgr->CreateVertexBuffer(vertices.size(), sizeof(float), i_info.m_ver_usage, &vertices[0]);
					HardwareIndexBuffer::IndexType ind_type = HardwareIndexBuffer::IndexType::Int;
					/*
					TODO: correct determination of vertex types
					if (indices.size() < 65535)
					ind_type = HardwareIndexBuffer::IndexType::Short;
					else if(indices.size() < 255)
					ind_type = HardwareIndexBuffer::IndexType::Byte;*/
					auto ind_buf = p_mgr->CreateIndexBuffer(ind_type, indices.size(), i_info.m_ind_usage, &indices[0]);
					auto ver_layout = p_mgr->CreateElement(3, Render::VertexSemantic::Position, Render::PrimitiveType::Triangles, Render::ComponentType::Float, false);

					return std::make_pair(LoadResult::Success, Mesh(ver_buf, ind_buf, ver_layout));
				}

				// TODO: need some limitations on size of loaded meshes?
				static int CreateNewHandle()
				{
					auto& handlers = Render::g_mesh_system.m_handlers;

					int new_index = -1;
					for (size_t i = 0; i < handlers.size(); ++i)
					{
						if (handlers[i].index == -1)
						{
							new_index = static_cast<int>(i);
							handlers[i].index = i;
							break;
						}
					}

					if (new_index == -1)
					{
						// TODO: resize vector normally - with some strategy - not push_back
						handlers.push_back(Render::MeshHandler());						
						new_index = handlers.size() - 1;

						handlers[new_index].generation = 0;
						handlers[new_index].index = new_index;
					}

					return new_index;
				}

				static void RemoveHandle(int i_handle)
				{
					auto& handlers = Render::g_mesh_system.m_handlers;
					assert(i_handle < static_cast<int>(handlers.size()));
					++handlers[i_handle].generation;
					handlers[i_handle].index = -1;
				}

				static void UnloadResource(int i_handle)
				{
					auto& handlers = Render::g_mesh_system.m_handlers;
					assert(i_handle < static_cast<int>(handlers.size()));
					++handlers[i_handle].generation;
					handlers[i_handle].index = -1;


					auto& meshes = Render::g_mesh_system.m_meshes;
					assert(i_handle < static_cast<int>(meshes.size()));
					// release resources for mesh
					auto p_mgr = Core::GetRenderer()->GetHardwareBufferMgr();
					Render::Mesh& mesh = meshes[i_handle];
					p_mgr->DestroyBuffer(mesh.GetVertices());
					p_mgr->DestroyBuffer(mesh.GetLayout());
					p_mgr->DestroyBuffer(mesh.GetIndices());
				}

				static void Register(int i_handle, Render::Mesh i_mesh)
				{					
					auto& meshes = Render::g_mesh_system.m_meshes;					
					if (static_cast<int>(meshes.size()) < i_handle)
					{
						// TODO: increase buffer strategy
						const size_t RANGE = 1000;
						float coeff = 2;
						if (meshes.size() > RANGE)
							coeff = 1.5f;
						meshes.resize(static_cast<size_t>(meshes.size() * coeff));
					}
					

					meshes[i_handle] = i_mesh;
				}
			};
		
		} // Serialization

	} // Resources

	namespace Render
	{
		static MeshSystem mesh_system;
		MeshSystem& g_mesh_system = mesh_system;

		MeshSystem::MeshSystem()
		{
			// TODO: choose initial size - provide generic strategy for allocating such types of buffers
			const size_t INITIAL_SIZE = 10;
			m_meshes.resize(INITIAL_SIZE);
			m_handlers.resize(INITIAL_SIZE);
			for (auto& handler : m_handlers)
				handler.index = -1;
		}

		MeshSystem::~MeshSystem()
		{

		}

		void MeshSystem::Update(float i_elapsed_time)
		{

		}

		void MeshSystem::SubmitDrawCommands()
		{
			auto& render_world = Core::GetApplication()->GetRenderWorld();

			for (auto& handler : m_component_handlers)
			{
				// reach the end of registered (valid) meshes
				if (handler.index == -1)
					break;

				auto& mesh_instance = m_instances[handler.index];
				auto& mesh = m_meshes[mesh_instance.GetHandler().index];

				Commands::Transform* p_transform_cmd = nullptr;
				auto p_entity = g_entity_manager.GetEntity(mesh_instance.GetEntity());
				if (p_entity != nullptr)
				{
					auto p_transform = p_entity->GetComponent<Transform>();
					// we have no transform - cannot draw somewhere -> skip
					if (p_transform == nullptr)
					{
						assert(false && "No transform component for mesh");
						continue;
					}
					p_transform_cmd = Render::gBuffer.Create<Commands::Transform>();
					p_transform_cmd->Translate(p_transform->m_position);
				}

				Commands::Draw* p_cmd = Render::gBuffer.Append<Commands::Draw>(p_transform_cmd);
				p_cmd->vertices = mesh.GetVertices();
				p_cmd->layout = mesh.GetLayout();
				p_cmd->indices = mesh.GetIndices();
			}
		}
		
		MeshHandler MeshSystem::Load(const std::string& i_file_name, Render::BufferUsageFormat i_vertices_usage, Render::BufferUsageFormat i_indices_usage)
		{
			MeshInformation info = { i_vertices_usage, i_indices_usage };
			int index = Resources::g_load_manager.Load<Mesh>(i_file_name, info);

			// resource is already loaded
			if (index != -1)
			{
				assert(index < static_cast<int>(m_handlers.size()));
				return m_handlers[index];
			}
			
			MeshHandler handler;
			handler.generation = 0;
			handler.index = -1;
			return handler;
		}

		void MeshSystem::Unload(MeshHandler i_handler)
		{
			Resources::g_load_manager.Unload<Mesh>(i_handler.index);
		}

		MeshComponentHandler MeshSystem::CreateInstance(MeshHandler i_handler)
		{
			static MeshComponentHandler error_handler{ -1, -1 };
			if (i_handler.generation != m_handlers[i_handler.index].generation)
				return error_handler;

			// get free index
			int new_index = -1;
			for (size_t i = 0; i < m_component_handlers.size(); ++i)
			{
				if (m_component_handlers[i].index == -1)
				{
					new_index = static_cast<int>(i);
					m_component_handlers[i].index = i;
					break;
				}
			}
			// create index
			if (new_index == -1)
			{
				// TODO: resize vector normally - with some strategy - not push_back
				const size_t new_count = m_component_handlers.size() + 1;
				m_component_handlers.resize(new_count);
				m_instances.resize(new_count);
				new_index = new_count - 1;
				m_component_handlers[new_index].generation = 0;
				m_component_handlers[new_index].index = new_index;

				
			}
			
			//////////////////
			// set data for MeshComponent			
			m_instances[new_index] = MeshComponent(i_handler);			
			assert(m_instances.size() == m_component_handlers.size());
			return m_component_handlers[new_index];
		}

		MeshComponent* MeshSystem::GetInstance(MeshComponentHandler i_handler)
		{
			if (m_component_handlers[i_handler.index].generation != i_handler.generation)
			{
				assert(false && "Generation is not identical");
				return nullptr;
			}
			return &m_instances[i_handler.index];
		}

		void MeshSystem::RemoveInstance(MeshComponentHandler i_handler)
		{
			m_component_handlers[i_handler.index].index = -1;
			++m_component_handlers[i_handler.index].generation;
		}

		///////////////////////////////////////////////////////////////////////////////
		// Extension for EntityManager

		MeshComponent* MeshSystem::Get(int i_in_system_id, int i_in_system_generation)
		{
			MeshComponentHandler inst_handler{ i_in_system_id, i_in_system_generation };
			MeshComponent* component = g_mesh_system.GetInstance(inst_handler);
			return component;
		}

		void MeshSystem::Remove(int i_in_system_id, int i_in_system_generation)
		{
			g_mesh_system.RemoveInstance({ i_in_system_id, i_in_system_generation });
		}
		

	} // Render

} // SDK