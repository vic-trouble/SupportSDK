#include "stdafx.h"

#include "MaterialManager.h"

#include "ShaderSystem.h"

#include "Core.h"
#include "Render/IRenderer.h"
#include "Render/TextureManager.h"
#include "Render/ShaderCompiler.h"

#include "Resources/ResourceManager.h"

#include "FileSystem/FileSystem.h"

#include "PropertyReader.h"

namespace SDK
{	
	namespace
	{
		using namespace Render;
		using namespace details;

		ShaderVariableType StringToSVT(const std::string& i_type)
		{
			using SVT = ShaderVariableType;
			if (i_type == "sampler2D")
				return SVT::Sampler2D;
			if (i_type == "float")
				return SVT::Float;
			if (i_type == "vec4")
				return SVT::FloatVec4;
			return SVT::Undefined;
		}

		ValueContainer GetPreprocessedValue(const PropertyElement& i_var_elem, ShaderVariableType i_type)
		{
			ValueContainer container;
			using SVT = ShaderVariableType;
			switch (i_type)
			{
				case SVT::Sampler2D:
					{
						auto p_mgr = Core::GetRenderer()->GetTextureManager();
						const auto p_tex_name = i_var_elem.GetValuePtr<std::string>("value");
						if (p_tex_name == nullptr)
						{
							assert(false);
							return std::move(container);
						}
						auto handle = p_mgr->Load(*p_tex_name);
						container.SetValue<TextureHandle>(0, handle);
					}
					break;
				case SVT::IntVec2:
				case SVT::IntVec3:
				case SVT::IntVec4:
					{
						const auto p_vec = i_var_elem.GetValuePtr<std::vector<int>>("value");
						if (p_vec == nullptr)
						{
							assert(false);
							break;
						}
						const size_t size = p_vec->size()*sizeof(int);
						container.SetValue(0, (const void*)&(*p_vec)[0], size);
					}
					break;
				case SVT::FloatVec2:
				case SVT::FloatVec3:
				case SVT::FloatVec4:
					{
						const auto p_vec = i_var_elem.GetValuePtr<std::vector<float>>("value");
						if (p_vec == nullptr)
						{
							assert(false);
							break;
						}
						const size_t size = p_vec->size()*sizeof(int);
						container.SetValue(0, (const void*)&(*p_vec)[0], size);
					}
					break;
				case SVT::DoubleVec2:
				case SVT::DoubleVec3:
				case SVT::DoubleVec4:
					{
						const auto p_vec = i_var_elem.GetValuePtr<std::vector<double>>("value");
						if (p_vec == nullptr)
						{
							assert(false);
							break;
						}
						const size_t size = p_vec->size()*sizeof(int);
						container.SetValue(0, (const void*)&(*p_vec)[0], size);
					}
					break;
				default:
					{
						const auto p_any = i_var_elem.GetAnyPtr("value");
						if (p_any == nullptr)
						{
							assert(false);
							break;
						}
						container.SetValue(0, p_any->get_raw_ptr(), p_any->get_size());
					}
					break;
			}
			return std::move(container);
		}

		std::vector<MaterialEntry> ParseShaderElement(const PropertyElement& i_shader_elem)
		{
			std::string shader_name = i_shader_elem.GetValue<std::string>("name");
			const auto p_shader = Render::g_shader_system.Access(shader_name);
			if (p_shader == nullptr)
				return std::vector<MaterialEntry>();

			std::vector<MaterialEntry> shader_properties;
			const auto end = i_shader_elem.end<PropertyElement>();
			for (auto it = i_shader_elem.begin<PropertyElement>(); it != end; ++it)
			{
				auto shader_var_name = it.element_name();
				auto uniform = p_shader->GetUniform(shader_var_name);
				// no such uniform
				if (uniform.location == -1)
				{
					assert(false);
					continue;
				}

				const PropertyElement& element = *it;
				const std::string* p_type = element.GetValuePtr<std::string>("type");
				if (p_type == nullptr)
				{
					// no type property
					assert(false);
					continue;
				}
				ShaderVariableType type = StringToSVT(*p_type);
				if (type == ShaderVariableType::Undefined)
				{
					// unknown type
					assert(false);
					continue;
				}

				std::string show_name = element.GetValue<std::string>("show_name");
				ValueContainer value = std::move(GetPreprocessedValue(element, type));

				shader_properties.emplace_back(uniform.location, std::move(value), type, show_name);
			}
			return std::move(shader_properties);
		}

		bool ParseMaterial(Material& o_material, const PropertyElement& i_material_elem)
		{
			const PropertyElement* p_shader_elem = i_material_elem.GetValuePtr<PropertyElement>("Shader");
			if (p_shader_elem == nullptr)
				return false;

			o_material.m_name = i_material_elem.GetValue<std::string>("name");
			o_material.m_name_hash = Utilities::hash_function(o_material.m_name);
			std::string shader_name = p_shader_elem->GetValue<std::string>("name");
			o_material.m_shader.index = Core::GetGlobalObject<Resources::ResourceManager>()->GetHandleToResource(shader_name).first;
			o_material.m_shader.generation = 0;
			if (o_material.m_shader.index == -1)
				return false;

			o_material.m_entries = std::move(ParseShaderElement(*p_shader_elem));

			return true;
		}

	}

	namespace Resources
	{
		namespace Serialization
		{
			template <>
			struct Definition <Render::Material>
			{
				typedef void* InfoType;
			};

			template <>
			struct LoaderImpl < Render::Material >
			{
				// TODO: is it possible to catch error like ip_streams[3] in compile time?
				static std::pair<LoadResult, Render::Material> Load(std::istream& i_stream, void*)
				{
					using namespace Render;
					Material material;
					
					PropretyReader<(int)ReaderType::SDKFormat> reader;
					PropertyElement root = reader.Parse(i_stream);
					const PropertyElement* p_material_elem = root.GetValuePtr<PropertyElement>("Material");
					if (p_material_elem == nullptr)
						return std::make_pair(LoadResult::Failure, material);
					const bool result = ParseMaterial(material, *p_material_elem);
					return std::make_pair(result ? LoadResult::Success : LoadResult::Failure, std::move(material));
				}

				static int CreateNewHandle()
				{
					auto handle = Render::g_material_mgr.m_materials.CreateNew();
					return handle.index;
				}

				static void RemoveHandle(int i_handle)
				{
					Render::g_material_mgr.m_materials.Destroy({ i_handle, 0 });
				}

				static void UnloadResource(int i_handle)
				{
					Render::g_material_mgr.m_materials.m_elements[i_handle].second = Render::Material();
					Render::g_material_mgr.m_materials.m_elements[i_handle].first.index = -1;
					Render::g_material_mgr.m_materials.m_elements[i_handle].first.generation;
				}

				static void Register(int i_handle, Render::Material i_material)
				{
					Render::g_material_mgr.m_materials.m_elements[i_handle].second = std::move(i_material);
				}
			};

		} // Serialization

	} // Resources


	namespace Render
	{
		static MaterialManager material_mgr;
		MaterialManager& g_material_mgr = material_mgr;
		

		MaterialHandle MaterialManager::Load(const std::string& i_resource_name, const std::string& i_path)
		{
			auto p_load_manager = Core::GetGlobalObject<Resources::ResourceManager>();
			int index = p_load_manager->Load<Material>(i_path, nullptr);

			// resource is already loaded
			if (index != -1)
			{
				assert(index >= 0 && index < static_cast<int>(m_materials.m_elements.size()));
				return m_materials.m_elements[index].first;
			}

			return MaterialHandle::InvalidHandle();
		}

		const Material* MaterialManager::AccessMaterial(MaterialHandle i_handle) const
		{
			return m_materials.Access(i_handle);
		}

		void MaterialManager::RemoveMaterial(MaterialHandle i_handle)
		{
			m_materials.Destroy(i_handle);
		}

		void MaterialManager::CreateCommand(MaterialHandle i_material, const void* ip_parent_command) const
		{

		}

	} // Render
} // SDK