#include "stdafx.h"
#include "ResourceManager.h"

#include "Core.h"
#include "Render/HardwareBufferManagerBase.h"
#include "Render/IRenderer.h"

#include "FileSystem/FileStream.h"

#include <fstream>

#include "PropertyReaders.h"

namespace SDK
{
	using namespace Render;

	namespace Resources
	{
		FS::StreamPtr ResourceManager::OpenStream(const std::string& i_file_name)
		{			
			FS::StreamPtr p_stream(new FS::FileStream(i_file_name));
			if (!p_stream->IsValid())
				return nullptr;
			return p_stream;
		}

		InternalHandle ResourceManager::GetHandleToResource(const std::string& i_res_name) const
		{
			const size_t hash = Utilities::hash_function(i_res_name);
			// check if handle already exist
			auto it = std::find_if(m_loaded_resources.begin(), m_loaded_resources.end(), ResourceInformation::FindPredicate(hash));
			if (it != m_loaded_resources.end())
				return{ it->m_handle.index, it->m_handle.generation };
			return InternalHandle::InvalidHandle();
		}

		void ResourceManager::Use(const std::string& i_res_name)
		{
			const size_t hash = Utilities::hash_function(i_res_name);
			// check if handle already exist
			auto it = std::find_if(m_loaded_resources.begin(), m_loaded_resources.end(), ResourceInformation::FindPredicate(hash));
			if (it != m_loaded_resources.end())
				++it->m_use_count;
			else
			{
				assert(false && "No resource with such handle");
			}
		}

		void Reload()
		{
			// object
			// method
		}

		void ResourceManager::Update(float i_dt)
		{
			// check all resources for changes
			// reload if nedeed
			Reload();
		}

		void ResourceManager::AddToSet(ResourceSetHandle i_set, size_t i_res_hash)
		{
			ResourceSet* p_set = m_resource_sets.Access(i_set);
			if (p_set == nullptr)
				return;
			if (p_set)
				p_set->subset.AddEntity(i_res_hash);
		}

		void ResourceManager::RemoveFromSet(ResourceSetHandle i_set, size_t i_res_hash)
		{
			ResourceSet* p_set = m_resource_sets.Access(i_set);
			if (p_set == nullptr)
				return;
			p_set->subset.RemoveEntity(i_res_hash);
		}

		ResourceSetHandle ResourceManager::CreateResourceSet(const std::string & i_resource_set_name)
		{
			auto handle = m_resource_sets.Create();
			SetCurrentResourceSet(handle);
			return handle;
		}

		ResourceSetHandle ResourceManager::LoadResourceSet(const std::string& i_resource_path)
		{
			PropretyReader<(int)ReaderType::SDKFormat> reader;
			PropertyElement root = reader.Parse(i_resource_path);

			std::string set_name = root.GetValue<std::string>("set_name");

			// create new resource set and set it to current
			auto handle = m_resource_sets.Create();
			m_resource_sets.m_elements[handle.index].second.name = set_name;
			m_resource_sets.m_elements[handle.index].second.name_hash = Utilities::hash_function(set_name);
			// last loaded will be active
			SetCurrentResourceSet(handle);

			const auto end = root.end<PropertyElement>();
			for (auto it = root.begin<PropertyElement>(); it != end; ++it)
				m_dispatcher.HandleMessage(*it, it.element_name());
			
			return handle;
		}

		ResourceSetHandle ResourceManager::GetHandleToSet(const std::string& i_set_name) const
		{
			const size_t hash = Utilities::hash_function(i_set_name);
			auto it = std::find_if(m_resource_sets.m_elements.begin(), m_resource_sets.m_elements.end(), [hash](const ResourceSets::ArrayElement& elem)
			{
				return elem.second.name_hash == hash;
			});
			if (it != m_resource_sets.m_elements.end())
				return it->first;

			return ResourceSetHandle::InvalidHandle();
		}

		void ResourceManager::SetCurrentResourceSet(const std::string& i_set_name)
		{
			SetCurrentResourceSet(GetHandleToSet(i_set_name));
		}

		void ResourceManager::SetCurrentResourceSet(ResourceSetHandle i_handle)
		{
			m_current_set = i_handle;
		}

		void ResourceManager::UnloadSet(ResourceSetHandle i_set)
		{
			ResourceSet* p_set = m_resource_sets.Access(i_set);
			if (p_set == nullptr)
			{
				assert(false && "This is not our set. Or incorrect");
				return;
			}

			for (size_t i_name_hash : p_set->subset.GetHandles())
			{
				auto res_it = std::find_if(m_loaded_resources.begin(), m_loaded_resources.end(), ResourceInformation::FindPredicate(i_name_hash));
				if (res_it == m_loaded_resources.end())
				{
					assert(false && "We found no loaded resources with such hash");
					continue;
				}

				auto& res_to_unload = *res_it;
				auto it = std::find_if(m_additional_functions.begin(), m_additional_functions.end(), [&res_to_unload](AdditionalFunctions& i_funcs)
				{
					return res_to_unload.m_resource_type == i_funcs.res_type;
				});
				// we have not found functions to unload
				if (it == m_additional_functions.end())
					continue;
				// handle that is inside specific system
				auto in_system_handle = res_to_unload.m_handle;
				it->Unload(in_system_handle);
			}

			m_resource_sets.Destroy(i_set);
		}

	} // Resources

} // SDK