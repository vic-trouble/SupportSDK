#include "stdafx.h"

#include "MeshSystem.h"

#include "Core.h"
#include "Render/Commands.h"
#include "Render/HardwareBufferManagerBase.h"
#include "Render/IRenderer.h"
#include "Applications/ApplicationBase.h"
#include "Render/RenderWorld.h"
#include "Render/MaterialManager.h"
#include "Resources/ResourceManager.h"

#include "Systems/TransformationsSystem.h"
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
				template <typename type, size_t size>
				std::istream& operator >> (std::istream& io_stream, Math::Vector<type, size>& o_vector)
				{
					//v 4.000000 0.000000 0.000000
					std::string temp;
					getline(io_stream, temp);

					size_t pos = 0;
					for (int i = 0; i < size; ++i)
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
				static std::pair<LoadResult, Render::Mesh> Load(std::istream* ip_streams[1], MeshInformation i_info)
				{
					using namespace Render;

					/////////////////
					std::vector<float> vertices;
					std::vector<float> uvs;
					std::vector<int> indices;					
					std::string model_name;
					/////////////////
					std::istream& stream = *ip_streams[0];
					Vector3 vec3;
					Vector3i vec3i;
					Vector2 vec2;
					while (ip_streams[0]->good())
					{
						char buffer[255];
						stream >> buffer;

						switch (buffer[0])
						{
							case 'v':
								if (buffer[1] == '\0')
								{
									stream >> vec3;
									for (size_t i = 0; i < 3; ++i)
										vertices.push_back(vec3[i]);
									break;
								}
								// texture
								if (buffer[1] == 't')
								{
									*ip_streams[0] >> vec2;
									uvs.push_back(vec2[0]);
									uvs.push_back(vec2[1]);
								}
								break;
							case 'o':
								stream >> model_name;
								break;
							case 'f':
								{
									if (buffer[1] != '\0')
										break;
									std::string temp;
									getline(stream, temp);

									size_t pos = 0;
									for (int i = 0; i < 3; ++i)
									{
										pos = temp.find(' ', pos) + 1;
										vec3i[i] = atoi(temp.substr(pos).c_str()) - 1;
										// textures
										pos = temp.find('/', pos) + 1;
										// normals
										pos = temp.find('/', pos) + 1;
									}
									for (size_t i = 0; i < 3; ++i)
										indices.push_back(vec3i[i]);

								}
								break;
							default:
								stream.getline(buffer, 254);
								break;
						}
					}
					
					auto p_mgr = Core::GetRenderer()->GetHardwareBufferMgr();
					auto ver_buf = p_mgr->CreateHardwareBuffer(vertices.size()*sizeof(float), i_info.m_ver_usage, &vertices[0]);
					auto uvs_buf = p_mgr->CreateHardwareBuffer(uvs.size()*sizeof(float), i_info.m_ver_usage, &uvs[0]);
					HardwareIndexBuffer::IndexType ind_type = HardwareIndexBuffer::IndexType::Int;
					/*
					TODO: correct determination of vertex types
					if (indices.size() < 65535)
					ind_type = HardwareIndexBuffer::IndexType::Short;
					else if(indices.size() < 255)
					ind_type = HardwareIndexBuffer::IndexType::Byte;*/
					auto ind_buf = p_mgr->CreateIndexBuffer(ind_type, indices.size(), i_info.m_ind_usage, Render::PrimitiveType::Triangles, &indices[0]);
					auto ver_layout = p_mgr->CreateLayout(ver_buf, 3, Render::VertexSemantic::Position, Render::ComponentType::Float, false, sizeof(float)*3/*cam be 0*/, 0);
					auto uv_layout = p_mgr->CreateLayout(uvs_buf, 2, Render::VertexSemantic::TextureCoordinates, Render::ComponentType::Float, false, sizeof(float) * 2/*cam be 0*/, 0);

					return std::make_pair(LoadResult::Success, Mesh(ver_buf, uvs_buf, ind_buf, ver_layout, uv_layout));
				}

				// TODO: need some limitations on size of loaded meshes?
				static InternalHandle CreateNewHandle()
				{
					auto& handlers = Render::g_mesh_system.m_handlers;

					InternalHandle new_index{ -1, -1 };
					for (size_t i = 0; i < handlers.size(); ++i)
					{
						if (handlers[i].index == -1)
						{
							new_index.index = i;
							new_index.generation = handlers[i].generation;
							handlers[i].index = i;
							break;
						}
					}

					if (new_index.index == -1)
					{
						// TODO: resize vector normally - with some strategy - not push_back
						handlers.push_back(Render::MeshHandler());
						const int index = handlers.size() - 1;						
						handlers[index].generation = 0;
						handlers[index].index = index;

						new_index.index = index;
						new_index.generation = 0;
					}

					return new_index;
				}

				static void RemoveHandle(InternalHandle i_handle)
				{
					auto& handlers = Render::g_mesh_system.m_handlers;
					assert(i_handle.index < static_cast<int>(handlers.size()));
					++handlers[i_handle.index].generation;
					handlers[i_handle.index].index = -1;
				}

				static void UnloadResource(InternalHandle i_handle)
				{
					auto& handlers = Render::g_mesh_system.m_handlers;
					assert(i_handle.index < static_cast<int>(handlers.size()));
					++handlers[i_handle.index].generation;
					handlers[i_handle.index].index = -1;

					auto& meshes = Render::g_mesh_system.m_meshes;
					assert(i_handle.index < static_cast<int>(meshes.size()));
					// release resources for mesh
					auto p_mgr = Core::GetRenderer()->GetHardwareBufferMgr();
					Render::Mesh& mesh = meshes[i_handle.index];
					p_mgr->DestroyBuffer(mesh.GetVertices());
					p_mgr->DestroyBuffer(mesh.GetUvs());
					p_mgr->DestroyLayout(mesh.GetLayout());
					p_mgr->DestroyBuffer(mesh.GetIndices());					
				}

				static void Register(InternalHandle i_handle, Render::Mesh i_mesh)
				{					
					auto& meshes = Render::g_mesh_system.m_meshes;					
					if (static_cast<int>(meshes.size()) < i_handle.index)
					{
						// TODO: increase buffer strategy
						const size_t RANGE = 1000;
						float coeff = 2;
						if (meshes.size() > RANGE)
							coeff = 1.5f;
						meshes.resize(static_cast<size_t>(meshes.size() * coeff));
					}
					

					meshes[i_handle.index] = i_mesh;
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

		void MeshSystem::Initialize()
		{
			auto p_load_manager = Core::GetGlobalObject<Resources::ResourceManager>();
			p_load_manager->RegisterLoader<MeshSystem, Mesh>(*this, &MeshSystem::Load, "mesh");
		}

		void MeshSystem::Release()
		{
			auto p_load_manager = Core::GetGlobalObject<Resources::ResourceManager>();
			p_load_manager->Unregister<MeshSystem, Mesh>("mesh");
		}

		BufferUsageFormat StringToFormat(const std::string& i_format)
		{
			if (i_format == "static")
				return BufferUsageFormat::Static;
			return BufferUsageFormat::Static;
		}

		void MeshSystem::Load(const PropertyElement& i_resource_element)
		{
			const std::string resource_name = i_resource_element.GetValue<std::string>("resource_name");
			const std::string path = i_resource_element.GetValue<std::string>("path");
			const std::string ver_usage = i_resource_element.GetValue<std::string>("vertices_usage");
			const std::string ind_usage = i_resource_element.GetValue<std::string>("indices_usage");
			BufferUsageFormat ver = StringToFormat(ver_usage);
			BufferUsageFormat ind = StringToFormat(ind_usage);
			Load(resource_name, path, ver, ind);
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

				// TODO: need dynamic here and not static :`(
				// for now buffer place for 6 unis
				Commands::SetupShader<2, 6>* p_shader_cmd = Render::gBuffer.Append<Commands::SetupShader<2, 6>>(p_transform_cmd);
				p_shader_cmd->m_layouts[0] = mesh.GetLayout();
				p_shader_cmd->m_layouts[1] = mesh.GetUVLayout();
				void* p_parent_cmd = (void*)p_shader_cmd;
				if (!mesh_instance.GetMaterials().empty())
				{
					auto material_handle = mesh_instance.GetMaterials()[0];					
					const auto p_material = Render::g_material_mgr.AccessMaterial(material_handle);
					p_shader_cmd->m_shader = p_material->m_shader;					
					for (auto& entry : p_material->m_entries)
					{
						if (entry.type != ShaderVariableType::Sampler2D)
							p_shader_cmd->SetValue(entry.shader_var_location, entry.type, entry.container.GetDataPtr(), entry.container.size, false);
					}
					
					p_parent_cmd = Render::g_material_mgr.SetupShaderAndCreateCommands(&p_shader_cmd->m_dynamic_uniforms[p_shader_cmd->current_value], 6 - p_shader_cmd->current_value, *p_material, p_shader_cmd);
				}

				Commands::Draw* p_cmd = Render::gBuffer.Append<Commands::Draw>(p_parent_cmd);
				p_cmd->vertices = mesh.GetVertices();
				p_cmd->layout = mesh.GetLayout();
				p_cmd->indices = mesh.GetIndices();
			}
		}
		
		MeshHandler MeshSystem::Load(const std::string& i_name, const std::string& i_path, Render::BufferUsageFormat i_vertices_usage, Render::BufferUsageFormat i_indices_usage)
		{
			MeshInformation info = { i_vertices_usage, i_indices_usage };
			auto p_load_manager = Core::GetGlobalObject<Resources::ResourceManager>();
			InternalHandle handle = p_load_manager->Load<Mesh>(i_name, { i_path }, info);

			// resource is already loaded
			if (handle.index != -1)
			{
				assert(handle.index < static_cast<int>(m_handlers.size()));
				return m_handlers[handle.index];
			}

			return MeshHandler::InvalidHandle();
		}

		void MeshSystem::Unload(MeshHandler i_handler)
		{
			auto p_load_manager = Core::GetGlobalObject<Resources::ResourceManager>();
			p_load_manager->Unload<Mesh>({ i_handler.index, i_handler.generation });
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
			// TODO: increase use count for mesh
			m_instances[new_index] = MeshComponent(i_handler);			
			assert(m_instances.size() == m_component_handlers.size());
			return m_component_handlers[new_index];
		}

		void MeshSystem::AddMaterialTo(MeshComponentHandler i_component, MaterialHandle i_material)
		{
			auto p_material = Render::g_material_mgr.AccessMaterial(i_material);
			if (p_material == nullptr)
			{
				assert(false && "There is no such material");
				return;
			}
			if (i_component.index >= static_cast<int>(m_component_handlers.size()) || i_component.generation != m_component_handlers[i_component.index].generation)
			{
				assert(false && "There is no such mesh component");
				return;
			}

			auto& mesh_instance = m_instances[i_component.index];
			mesh_instance.AddMaterial(i_material);
		}

		MeshComponent* MeshSystem::AccessComponent(MeshComponentHandler i_handler)
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
			// TODO: decrease use count for mesh
			m_component_handlers[i_handler.index].index = -1;
			++m_component_handlers[i_handler.index].generation;
		}

		///////////////////////////////////////////////////////////////////////////////
		// Extension for EntityManager

		MeshComponent* MeshSystem::Get(int i_in_system_id, int i_in_system_generation)
		{
			MeshComponentHandler inst_handler{ i_in_system_id, i_in_system_generation };
			MeshComponent* component = g_mesh_system.AccessComponent(inst_handler);
			return component;
		}

		void MeshSystem::Remove(int i_in_system_id, int i_in_system_generation)
		{
			g_mesh_system.RemoveInstance({ i_in_system_id, i_in_system_generation });
		}
		

	} // Render

} // SDK