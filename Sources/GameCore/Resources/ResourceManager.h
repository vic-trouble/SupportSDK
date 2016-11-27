#ifndef __GAMECORE_RESOURCEMANAGER_H__
#define __GAMECORE_RESOURCEMANAGER_H__

#include "../GameCoreAPI.h"

#include "../GlobalObjectBase.h"

#include "../ObjectSubset.h"
#include "../GenericHandlesStaticArray.h"
#include "ResourceInformation.h"
#include "../PropertyElement.h"
#include "../FileSystem/Stream.h"

#include <Utilities/HashFunctions.h>
#include <Patterns/MessageDispatcher/MessageDispatcher.h>

namespace SDK
{

	namespace Resources
	{
		namespace Serialization
		{
			
			template <typename ResType>
			struct Definition
			{	};

			enum class LoadResult
			{
				Success,
				Failure
			};

			template <typename ResType>
			struct LoaderImpl
			{
				using InfoType = typename Definition<ResType>::InfoType;
				using ResourceType = ResType;
				using RawData = typename Definition<ResType>::RawDataType;

				static std::pair<LoadResult, ResourceType> Load(std::istream*, InfoType);
				static std::pair<LoadResult, ResourceType> Load(RawData&&);
				static int CreateNewHandle();
				static void RemoveHandle(InternalHandle i_handle);
				static void UnloadResource(InternalHandle i_handle);
				static void Register(InternalHandle i_handle, ResType i_resource);
			};

		} // Serialization
		
		using LoadedResPair = std::pair<size_t, std::type_index>;
		using ResourcesSubset = ObjectSet<LoadedResPair>;

		class ResourceManager : public GlobalObjectBase
		{
		private:
			std::vector<ResourceInformation> m_loaded_resources;
			MessageDispatcherBase<PropertyElement> m_dispatcher;
			struct ResourceSet
			{
				ResourcesSubset subset;
				std::string name;
				size_t name_hash;
				ResourceSet() = default;
				ResourceSet(const std::string& i_name)
					: name(i_name)
					, name_hash(Utilities::hash_function(i_name))
				{}
			};

			using ResourceSets = SDK::GenericHandlesStaticArray<ResourceSetHandle, ResourceSet, 128>;
			ResourceSets m_resource_sets;
			ResourceSetHandle m_current_set;

			struct AdditionalFunctions
			{
				std::type_index res_type;
				void(*Unload)(InternalHandle);
				//void(*Reload)(InternalHandle);
			};
			std::vector<AdditionalFunctions> m_additional_functions;
		private:
			static FS::StreamPtr OpenStream(const std::string& i_file_name);

			// already registered internally - reload and increase counter
			template <typename ResType>
			void Register(ResourceInformation& io_info, ResType i_resource)
			{
				Serialization::LoaderImpl<ResType>::Register(io_info.m_handle, std::move(i_resource));
				++io_info.m_use_count;
			}

			void AddToSet(ResourceSetHandle i_set, LoadedResPair i_res_hash);
			void RemoveFromSet(ResourceSetHandle i_set, LoadedResPair i_res_hash);
			GAMECORE_EXPORT InternalHandle GetHandleToResource(const std::string& i_res_name, const std::type_index& i_res_type) const;
			GAMECORE_EXPORT void Use(const std::string& i_res_name, const std::type_index& i_res_type);

		public:
			ResourceManager()
				: m_current_set(ResourceSetHandle::InvalidHandle())
			{}
			// TODO: comments
			// returns handler to resource if success, -1 otherwise
			// For systems it can be transformed to Handler<>
			// if one of i_file_names is empty than on that index nullptr pointer will be set
			template <typename ResType, size_t parts_count = 1>
			InternalHandle Load(
				const std::string& i_res_name,
				std::initializer_list<std::string> i_file_names,
				typename Serialization::Definition<ResType>::InfoType i_info
				)
			{
				using namespace Serialization;
				assert(parts_count == i_file_names.size());
				// TODO: is it needed?
				if (parts_count != i_file_names.size())
					return InternalHandle::InvalidHandle();
				///	
				using Loader = LoaderImpl<ResType>;
				const std::type_index res_type = typeid(ResType);
				const size_t hash = Utilities::hash_function(i_res_name);
				// check if handle already exist
				auto it = std::find_if(m_loaded_resources.begin(), m_loaded_resources.end(), ResourceInformation::FindByHash(hash, res_type));
				if (it != m_loaded_resources.end())
				{
					++it->m_use_count;
					return it->m_handle;
				}
				// handle not exist - this file was not loaded before			
				InternalHandle handle = Loader::CreateNewHandle();
				{
					ResourceInformation res_info = { hash, 0, ResourceInformation::State::Unloaded, handle, res_type, m_current_set };
					m_loaded_resources.emplace_back(res_info);
				}

				FS::StreamPtr streams[parts_count];
				std::istream* p_streams[parts_count];
				for (size_t i = 0; i < parts_count; ++i)
				{
					const std::string& file_name = *(i_file_names.begin() + i);
					// optional part and empty
					if (file_name.empty())
					{
						p_streams[i] = nullptr;
						continue;
					}
					streams[i] = OpenStream(file_name);
					if (streams[i] == nullptr)
					{
						Loader::RemoveHandle(handle);
						m_loaded_resources.erase(
							std::find_if(m_loaded_resources.begin(), m_loaded_resources.end(), ResourceInformation::FindByHash(hash, res_type)),
							m_loaded_resources.end());
						return InternalHandle::InvalidHandle();
					}

					p_streams[i] = &streams[i]->Get();
				}

				auto load_res = Loader::Load(p_streams, i_info);

				if (load_res.first == LoadResult::Failure)
				{
					Loader::RemoveHandle(handle);
					m_loaded_resources.erase(
						std::find_if(m_loaded_resources.begin(), m_loaded_resources.end(), ResourceInformation::FindByHash(hash, res_type)),
						m_loaded_resources.end());
					return InternalHandle::InvalidHandle();
				}

				// find just created resource
				it = std::find_if(m_loaded_resources.begin(), m_loaded_resources.end(), ResourceInformation::FindByHash(hash, res_type));
				if (it == m_loaded_resources.end())
				{
					Loader::RemoveHandle(handle);
					return InternalHandle::InvalidHandle();
				}
				auto& res_info = *it;
				res_info.m_state = ResourceInformation::State::Loaded;
				Register<ResType>(res_info, std::move(load_res.second));
				// add to current set
				AddToSet(res_info.m_belongs_to_set, { hash, res_type });
				return handle;
			}

			template <typename ResType>
			InternalHandle LoadFromData(
				const std::string& i_res_name,
				typename Serialization::Definition<ResType>::RawData&& i_data)
			{
				using namespace Serialization;
				using Loader = LoaderImpl<ResType>;
				const std::type_index res_type = typeid(ResType);
				const size_t hash = Utilities::hash_function(i_res_name);
				// check if handle already exist
				auto it = std::find_if(m_loaded_resources.begin(), m_loaded_resources.end(), ResourceInformation::FindByHash(hash, res_type));
				if (it != m_loaded_resources.end())
				{
					++it->m_use_count;
					return it->m_handle;
				}
				// handle not exist - this file was not loaded before			
				InternalHandle handle = Loader::CreateNewHandle();
				{
					ResourceInformation res_info = { hash, 0, ResourceInformation::State::Unloaded, handle, res_type, m_current_set };
					m_loaded_resources.emplace_back(res_info);
				}

				auto load_res = Loader::Load(std::move(i_data));

				if (load_res.first == LoadResult::Failure)
				{
					Loader::RemoveHandle(handle);
					m_loaded_resources.erase(
						std::find_if(m_loaded_resources.begin(), m_loaded_resources.end(), ResourceInformation::FindByHash(hash, res_type)),
						m_loaded_resources.end());
					return InternalHandle::InvalidHandle();
				}
				
				// find just created resource
				it = std::find_if(m_loaded_resources.begin(), m_loaded_resources.end(), ResourceInformation::FindByHash(hash, res_type));
				if (it == m_loaded_resources.end())
				{
					Loader::RemoveHandle(handle);
					return InternalHandle::InvalidHandle();
				}
				auto& res_info = *it;
				res_info.m_state = ResourceInformation::State::Loaded;
				Register<ResType>(res_info, std::move(load_res.second));
				// add to current set
				AddToSet(res_info.m_belongs_to_set, { hash, res_type });
				return handle;

			}

			template <typename ResType>
			void Unload(InternalHandle i_handle)
			{
				auto it = std::find_if(m_loaded_resources.begin(), m_loaded_resources.end(), ResourceInformation::FindByHandle(i_handle, typeid(ResType)));
				// <PANIC> Who creates this? It`s not ours; so we do nothing!
				if (it == m_loaded_resources.end())
				{
					assert(false && "It is not our Mesh");
					return;
				}

				--it->m_use_count;
				// the last one -> delete buffers
				if (it->m_use_count == 0)
				{					
					Serialization::LoaderImpl<ResType>::UnloadResource(it->m_handle);
					RemoveFromSet(it->m_belongs_to_set, { it->m_resource_id, it->m_resource_type });
					m_loaded_resources.erase(it, m_loaded_resources.end());					
				}
				// otherwise we do nothing
			}

			template <typename ResType>
			typename Serialization::Definition<ResType>::HandleType GetHandleToResource(const std::string& i_res_name) const
			{
				auto internal_handle = GetHandleToResource(i_res_name, typeid(ResType));
				return Serialization::Definition<ResType>::HandleType{internal_handle.index, internal_handle.generation};
			}
			
			template <typename ResType>
			void Use(const std::string& i_res_name)
			{
				Use(i_res_name, typeid(ResType));
			}			

			void Update(float i_dt);
			
			template <typename LoadSystem, typename ResType>
			void RegisterLoader(LoadSystem& i_instance, void (LoadSystem::*member_function)(const PropertyElement&), const std::string& i_resource_id)
			{
				m_dispatcher.RegisterHandler<LoadSystem, const PropertyElement&>(i_instance, member_function, "ResourceMgr", i_resource_id);
				using Loader = Serialization::LoaderImpl<ResType>;
				m_additional_functions.push_back({ std::type_index(typeid(ResType)), &Loader::UnloadResource });
			}
			template <typename LoadSystem, typename ResType>
			void Unregister(const std::string& i_resource_id)
			{
				m_dispatcher.UnregisterHandler<const PropertyElement&>("ResourceMgr", i_resource_id);
				std::type_index ti = typeid(ResType);
				auto it = std::find_if(m_additional_functions.begin(), m_additional_functions.end(), [ti](AdditionalFunctions& i_funcs)
				{
					return ti == i_funcs.res_type;
				});
				// we have not found functions to unload
				if (it != m_additional_functions.end())
					m_additional_functions.erase(it);
			}

			// last loaded set will be active
			GAMECORE_EXPORT ResourceSetHandle CreateResourceSet(const std::string& i_resource_set_name);
			GAMECORE_EXPORT ResourceSetHandle LoadResourceSet(const std::string& i_resource_path);
			GAMECORE_EXPORT ResourceSetHandle GetHandleToSet(const std::string& i_set_name) const;
			GAMECORE_EXPORT void UnloadSet(ResourceSetHandle i_set);
			GAMECORE_EXPORT void SetCurrentResourceSet(ResourceSetHandle i_handle);
			GAMECORE_EXPORT void SetCurrentResourceSet(const std::string& i_set_name);
		};

	} // Resources

} // SDK

#endif