#ifndef __GAMECORE_RESOURCEMANAGER_H__
#define __GAMECORE_RESOURCEMANAGER_H__

#include "../GameCoreAPI.h"

#include "../GlobalObjectBase.h"

#include "ResourceInformation.h"
#include "../FileSystem/Stream.h"
#include "../Render/Mesh.h"

#include <Utilities/HashFunctions.h>

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
				typedef typename Definition<ResType>::InfoType InfoType;
				typedef ResType ResourceType;

				static std::pair<LoadResult, ResourceType> Load(std::istream&, InfoType);
				static std::pair<LoadResult, ResourceType> Load(std::istream*, InfoType);
				static int CreateNewHandle();
				static void RemoveHandle(int i_handle);
				static void UnloadResource(int i_handle);
				static void Register(int i_handle, ResType i_resource);				
			};

		} // Serialization

		class ResourceManager : public GlobalObjectBase
		{
		private:
			std::vector<ResourceInformation> m_loaded_resources;

		private:
			static Render::Mesh LoadObj(const std::string& i_file_name, Render::BufferUsageFormat i_vertices_usage, Render::BufferUsageFormat i_indices_usage);

			static FS::StreamPtr OpenStream(const std::string& i_file_name);

			// already registered internally - reload and increase counter
			template <typename ResType>
			void Register(ResourceInformation& io_info, ResType i_resource)
			{
				Serialization::LoaderImpl<ResType>::Register(io_info.m_handle, i_resource);
				++io_info.m_use_count;
			}

		public:

			template <typename ResType, size_t parts_count>
			int Load(
				const std::string& i_res_name,
				std::initializer_list<std::string> i_file_names,
				typename Serialization::Definition<ResType>::InfoType i_info				
				)
			{
				using namespace Serialization;
				assert(parts_count == i_file_names.size());
				// TODO: is it needed?
				if (parts_count != i_file_names.size())
					return -1;
				///	
				using Loader = LoaderImpl<ResType>;

				const size_t hash = Utilities::hash_function(i_res_name);
				// check if handle already exist
				auto it = std::find_if(m_loaded_resources.begin(), m_loaded_resources.end(), ResourceInformation::FindPredicate(hash));
				if (it != m_loaded_resources.end())
				{
					++it->m_use_count;
					return it->m_handle;
				}
				// handle not exist - this file was not loaded before			
				const int handle = Loader::CreateNewHandle();
				{
					ResourceInformation res_info = { hash, 0, ResourceInformation::State::Unloaded, handle };
					m_loaded_resources.emplace_back(res_info);
				}

				FS::StreamPtr streams[parts_count];
				std::istream* p_streams[parts_count];
				for (size_t i = 0; i < parts_count; ++i)
				{
					streams[i] = OpenStream(*(i_file_names.begin() + i));
					if (streams[i] == nullptr)
					{
						Loader::RemoveHandle(handle);
						m_loaded_resources.erase(
							std::find_if(m_loaded_resources.begin(), m_loaded_resources.end(), ResourceInformation::FindPredicate(hash)),
							m_loaded_resources.end());
						return -1;
					}

					p_streams[i] = &streams[i]->Get();
				}
								
				auto load_res = Loader::Load(p_streams, i_info);

				if (load_res.first == LoadResult::Failure)
				{
					Loader::RemoveHandle(handle);
					m_loaded_resources.erase(
						std::find_if(m_loaded_resources.begin(), m_loaded_resources.end(), ResourceInformation::FindPredicate(hash)),
						m_loaded_resources.end());
					return -1;
				}

				// TODO: when will be async - this will not be reference to back element
				// auto it = std::find(m_loaded_resources.begin(), m_loaded_resources.end(), ResourceInformation::FindPredicate(hash));
				auto& res_info = m_loaded_resources.back();
				res_info.m_state = ResourceInformation::State::Loaded;
				Register<ResType>(res_info, load_res.second);

				return handle;
			}


			// TODO: comments
			// returns handler to resource if success, -1 otherwise
			// For systems it can be transformed to Handler<>

			// for now: i_file_name - relative path which depends on executable
			template <typename ResType>
			int Load(const std::string& i_file_name,
					typename Serialization::Definition<ResType>::InfoType i_info)
			{
				using namespace Serialization;
				const size_t hash = Utilities::hash_function(i_file_name);
				// check if handle already exist
				auto it = std::find_if(m_loaded_resources.begin(), m_loaded_resources.end(), ResourceInformation::FindPredicate(hash));
				if (it != m_loaded_resources.end())
				{
					++it->m_use_count;
					return it->m_handle;
				}
				// handle not exist - this file was not loaded before			
				const int handle = LoaderImpl<ResType>::CreateNewHandle();
				{
					ResourceInformation res_info = { hash, 0, ResourceInformation::State::Unloaded, handle };
					m_loaded_resources.emplace_back(res_info);
				}

				auto p_stream = OpenStream(i_file_name);
				if (p_stream == nullptr)
				{
					LoaderImpl<ResType>::RemoveHandle(handle);
					m_loaded_resources.erase(
						std::find_if(m_loaded_resources.begin(), m_loaded_resources.end(), ResourceInformation::FindPredicate(hash)),
						m_loaded_resources.end());
					return -1;
				}

				auto load_res = LoaderImpl<ResType>::Load(p_stream->Get(), i_info);

				if (load_res.first == LoadResult::Failure)
				{
					LoaderImpl<ResType>::RemoveHandle(handle);
					m_loaded_resources.erase(
						std::find_if(m_loaded_resources.begin(), m_loaded_resources.end(), ResourceInformation::FindPredicate(hash)),
						m_loaded_resources.end());
					return -1;
				}

				// TODO: when will be async - this will not be reference to back element
				// auto it = std::find(m_loaded_resources.begin(), m_loaded_resources.end(), ResourceInformation::FindPredicate(hash));
				auto& res_info = m_loaded_resources.back();
				res_info.m_state = ResourceInformation::State::Loaded;
				Register<ResType>(res_info, load_res.second);

				return handle;
			}			

			template <typename ResType>
			void Register(int i_handle, const std::string& i_name, ResType i_resource)
			{
				const size_t hash = hash_function(i_name);
				auto it = std::find_if(m_loaded_resources.begin(), m_loaded_resources.end(), ResourceInformation::FindPredicate(i_handle));
				if (it == m_loaded_resources.end())
				{
					Serialization::LoaderImpl<ResType>::Register(i_handle, i_resource);
					m_loaded_resources.emplace_back(hash, 1, ResourceInformation::State::Loaded, i_handle);
				}
				else
				{
					++it->m_use_count;
				}
			}

			template <typename ResType>
			void Unload(int i_handle)
			{
				auto it = std::find_if(m_loaded_resources.begin(), m_loaded_resources.end(), ResourceInformation::FindPredicate(i_handle));
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
					m_loaded_resources.erase(it, m_loaded_resources.end());
				}
				// otherwise we do nothing
			}
		};

	} // Resources

} // SDK

#endif