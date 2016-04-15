#ifndef	__GAMECORE_SHADERSYSTEM_H__
#define __GAMECORE_SHADERSYSTEM_H__

#include "../GameCoreAPI.h"
#include "GlobalObjectBase.h"

#include "RenderTypes.h"

#include "Shader.h"
#include "../PropertyElement.h"

namespace SDK
{
	namespace Render {
		class ShaderCompiler;
	}

	namespace Resources {
		namespace Serialization {
			template <typename ResType>
			struct Definition;
			template <>
			struct Definition <Render::Shader>
			{
				typedef Render::ShaderCompiler* InfoType;
				typedef Render::ShaderHandle HandleType;
			};

			template <typename ResInfo>
			struct LoaderImpl;
		} // Serialization
	} // Resources

	namespace Render
	{
		class ShaderSystem : public GlobalObjectBase
		{
			friend struct Resources::Serialization::LoaderImpl<Shader>;
		private:
			// TODO: limitations to 1024 shader loaded - is it enough?
			typedef GenericBuffersArray<ShaderHandle, Shader, 1024> Shaders;
			Shaders m_shaders;
			ShaderCompiler* mp_current_shader_compiler;
		public:
			ShaderSystem();
			struct ShaderSource
			{
				std::string vertex_shader_file;
				std::string tess_control_shader_file;
				std::string tess_compute_shader_file;
				std::string geometry_shader_file;
				std::string fragment_shader_file;
				std::string compute_shader_file;
				bool empty;
				using ShaderSourceEntry = std::pair<Shader::ShaderType, std::string>;
				ShaderSource(std::initializer_list<ShaderSourceEntry> i_entries)
					: empty(true)
				{
					for (auto entry : i_entries)
						AddEntry(entry);
				}
				inline void AddEntry(ShaderSourceEntry i_entry)
				{
					empty = false;
					switch (i_entry.first)
					{
						case Shader::Vertex:
							vertex_shader_file = std::move(i_entry.second);
							break;
						case Shader::TesselationConrol:
							tess_control_shader_file = std::move(i_entry.second);
							break;
						case Shader::TesselationEvaluation:
							tess_compute_shader_file = std::move(i_entry.second);
							break;
						case Shader::Geometry:
							geometry_shader_file = std::move(i_entry.second);
							break;
						case Shader::Fragment:
							fragment_shader_file = std::move(i_entry.second);
							break;
						case Shader::Compute:
							compute_shader_file = std::move(i_entry.second);
							break;
					}
				}
			};
			GAMECORE_EXPORT ShaderHandle Load(const std::string& i_resource_name, ShaderSource i_source);
			GAMECORE_EXPORT void Unload(ShaderHandle i_handler);
			GAMECORE_EXPORT const Shader* Access(const std::string& i_name) const;
			inline const Shader* Access(ShaderHandle i_handler) const
			{
				if (i_handler.index == -1 || i_handler.generation != m_shaders.m_handlers[i_handler.index].generation)
					return nullptr;
				return &m_shaders.m_buffer[i_handler.index];
			}

			GAMECORE_EXPORT void SetKnownUniforms(ShaderHandle i_handle);
			GAMECORE_EXPORT void SetUniform(ShaderHandle i_handle, const std::string& i_var_name, const ShaderUniformValue& i_value) const;
			GAMECORE_EXPORT void SetUniform(int i_location, const ShaderUniformValue& i_value) const;

			void Initialize();
			void Release();
			void Load(const PropertyElement& i_resource_element);
		};

	} // Render

} // SDK

#endif
