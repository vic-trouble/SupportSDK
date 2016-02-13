#include "stdafx.h"

#include "ShaderSystem.h"

#include "Core.h"
#include "Render/Commands.h"
#include "Render/ShaderCompiler.h"
#include "Render/IRenderer.h"
#include "Render/Shader.h"
#include "Applications/ApplicationBase.h"
#include "Render/RenderWorld.h"
#include "Resources/ResourceManager.h"

#include "FileSystem/FileSystem.h"

namespace SDK
{

	namespace Resources
	{
		namespace Serialization
		{
			template <>
			struct Definition <Render::Shader>
			{
				typedef void* InfoType;
			};

			template <>
			struct LoaderImpl < Render::Shader >
			{
				// TODO: is it possible to catch error like ip_streams[3] in compile time?
				static std::pair<LoadResult, Render::Shader> Load(std::istream* ip_streams[Render::Shader::TypesNumber], void*)
				{
					using namespace Render;
					auto p_compiler = Core::GetRenderer()->GetShaderCompiler();

					std::string shader_sources[Render::Shader::TypesNumber];
					for (size_t i = 0; i < Render::Shader::TypesNumber; ++i)
					{
						// check for optional
						if (ip_streams[i] == nullptr)
						{
							// ensure that we will have empty source
							shader_sources[i].clear();
							continue;
						}
						shader_sources[i] = FS::ReadFileToString(*ip_streams[i]);
					}
					const char* p_vertex = !shader_sources[Shader::Vertex].empty() ? shader_sources[Shader::Vertex].c_str() : nullptr;
					const char* p_tess_control = !shader_sources[Shader::TesselationConrol].empty() ? shader_sources[Shader::TesselationConrol].c_str() : nullptr;
					const char* p_tess_eval = !shader_sources[Shader::TesselationEvaluation].empty() ? shader_sources[Shader::TesselationEvaluation].c_str() : nullptr;
					const char* p_geom = !shader_sources[Shader::Geometry].empty() ? shader_sources[Shader::Geometry].c_str() : nullptr;
					const char* p_fragment = !shader_sources[Shader::Fragment].empty() ? shader_sources[Shader::Fragment].c_str() : nullptr;
					const char* p_compute = !shader_sources[Shader::Compute].empty() ? shader_sources[Shader::Compute].c_str() : nullptr;

					Shader shader = p_compiler->Compile(p_vertex, p_tess_control, p_tess_eval, p_geom, p_fragment, p_compute);
					return std::make_pair(shader.IsValid() ? LoadResult::Success : LoadResult::Failure, shader);
				}

				static int CreateNewHandle()
				{
					auto cur_index = Render::g_shader_system.m_shaders.m_current_index;
					auto& handlers = Render::g_shader_system.m_shaders.m_handlers;
					// TODO: if maximum we can hold
					//	try to find free buffers
					if (cur_index == Render::ShaderSystem::Shaders::Size)
					{
						for (size_t i = 0; i < Render::ShaderSystem::Shaders::Size; ++i)
						{
							if (handlers[i].index == -1)
							{
								cur_index = static_cast<int>(i);
								handlers[i].index = i;
								break;
							}
						}
						// TODO: hold error
						//	not found
						if (cur_index == -1)
							return -1;
					}					
					else
					{				
						// increase otherwise
						++Render::g_shader_system.m_shaders.m_current_index;
					}

					handlers[cur_index].index = cur_index;
					return handlers[cur_index].index;
				}

				static void RemoveHandle(int i_handle)
				{
					auto& handlers = Render::g_shader_system.m_shaders.m_handlers;
					assert(i_handle < static_cast<int>(Render::ShaderSystem::Shaders::Size));
					++handlers[i_handle].generation;
					handlers[i_handle].index = -1;
				}

				static void UnloadResource(int i_handle)
				{
					using namespace Render;

					auto& handlers = Render::g_shader_system.m_shaders;
					assert(i_handle < static_cast<int>(Render::ShaderSystem::Shaders::Size));
					++handlers.m_handlers[i_handle].generation;
					handlers.m_handlers[i_handle].index = -1;

					auto p_compiler = Core::GetRenderer()->GetShaderCompiler();
					p_compiler->Release(handlers.m_buffer[i_handle]);					
				}

				static void Register(int i_handle, Render::Shader i_shader)
				{
					Render::g_shader_system.m_shaders.m_buffer[i_handle] = i_shader;
				}
			};

		} // Serialization

	} // Resources

	namespace Render
	{

		ShaderSystem g_shader_system;

		ShaderSystem::ShaderSystem()
		{
			for (size_t i = 0; i < Render::ShaderSystem::Shaders::Size; ++i)
			{
				m_shaders.m_handlers[i].index = -1;
				m_shaders.m_handlers[i].generation = 0;
			}
		}

		ShaderHandler ShaderSystem::Load(const std::string& i_resource_name, ShaderSource i_source)
		{
			auto p_load_manager = Core::GetGlobalObject<Resources::ResourceManager>();
			int index = p_load_manager->Load<Shader, Shader::TypesNumber>(i_resource_name, 
				{ i_source.vertex_shader_file, i_source.tess_control_shader_file, i_source.tess_compute_shader_file,
					i_source.geometry_shader_file, i_source.fragment_shader_file, i_source.compute_shader_file }, 
				nullptr);

			// resource is already loaded
			if (index != -1)
			{
				assert(index < static_cast<int>(Render::ShaderSystem::Shaders::Size));
				return m_shaders.m_handlers[index];
			}

			ShaderHandler handler;
			handler.generation = 0;
			handler.index = -1;
			return handler;
		}

		void ShaderSystem::Unload(ShaderHandler i_handler)
		{
			auto p_load_manager = Core::GetGlobalObject<Resources::ResourceManager>();
			p_load_manager->Unload<Shader>(i_handler.index);
		}

	} // Render

} // SDK