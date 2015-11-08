#ifndef __GAMECORE_LOADMANAGER_H__
#define __GAMECORE_LOADMANAGER_H__

#include "../GameCoreAPI.h"

#include "ResourceInformation.h"

#include "../Render/Mesh.h"

namespace SDK
{
	
	namespace FS
	{
		struct Stream
		{
			virtual ~Stream(){}
			virtual std::istream& Get() = 0;
			virtual bool IsValid() const = 0;
		};

		typedef std::unique_ptr<Stream> StreamPtr;
	}

	namespace Resources
	{
		size_t hash_function(const std::string& str);		

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
				static int CreateNewHandle();
				static void RemoveHandle(int i_handle);
				static void UnloadResource(int i_handle);
				static void Register(int i_handle, ResType i_resource);
			};

		} // Serialization

		

		class LoadManager
		{
		private:
			typedef ResourceInformation<Render::Mesh> MeshResourceInfo;
			std::vector<MeshResourceInfo> m_loaded_resources;

			std::vector<ResInformation> m_loaded_resources_new;

		private:
			static Render::Mesh LoadObj(const std::string& i_file_name, Render::BufferUsageFormat i_vertices_usage, Render::BufferUsageFormat i_indices_usage);

			static FS::StreamPtr OpenStream(const std::string& i_file_name);

			// already registered internally - reload and increase counter
			template <typename ResType>
			void Register(ResInformation& io_info, ResType i_resource)
			{
				Serialization::LoaderImpl<ResType>::Register(io_info.m_handle, i_resource);
				++io_info.m_use_count;
			}

		public:
			// TODO: comments
			// returns handler to resource if success, -1 otherwise
			// For systems it can be transformed to Handler<>

			// for now: i_file_name - relative path which depends on executable
			template <typename ResType>
			int Load(const std::string& i_file_name,
					typename Serialization::Definition<ResType>::InfoType i_info)
			{
				using namespace Serialization;
				const size_t hash = hash_function(i_file_name);
				// check if handle already exist
				auto it = std::find_if(m_loaded_resources_new.begin(), m_loaded_resources_new.end(), ResInformation::FindPredicate(hash));
				if (it != m_loaded_resources_new.end())
				{
					++it->m_use_count;
					return it->m_handle;
				}
				// handle not exist - this file was not loaded before			
				const int handle = LoaderImpl<ResType>::CreateNewHandle();
				{
					ResInformation res_info = { hash, 0, ResInformation::State::Unloaded, handle };
					m_loaded_resources_new.emplace_back(res_info);
				}

				auto p_stream = OpenStream(i_file_name);
				if (p_stream == nullptr)
				{
					LoaderImpl<ResType>::RemoveHandle(handle);
					m_loaded_resources_new.erase(
						std::find_if(m_loaded_resources_new.begin(), m_loaded_resources_new.end(), ResInformation::FindPredicate(hash)),
						m_loaded_resources_new.end());
					return -1;
				}

				auto load_res = LoaderImpl<ResType>::Load(p_stream->Get(), i_info);

				if (load_res.first == LoadResult::Failure)
				{
					LoaderImpl<ResType>::RemoveHandle(handle);
					m_loaded_resources_new.erase(
						std::find_if(m_loaded_resources_new.begin(), m_loaded_resources_new.end(), ResInformation::FindPredicate(hash)),
						m_loaded_resources_new.end());
					return -1;
				}

				// TODO: when will be async - this will not be reference to back element
				// auto it = std::find(m_loaded_resources_new.begin(), m_loaded_resources_new.end(), ResInformation::FindPredicate(hash));
				auto& res_info = m_loaded_resources_new.back();
				Register<ResType>(res_info, load_res.second);

				return handle;
			}			

			template <typename ResType>
			void Register(int i_handle, const std::string& i_name, ResType i_resource)
			{
				const size_t hash = hash_function(i_name);
				Serialization::LoaderImpl<ResType>::Register(i_handle, i_resource);
				auto it = std::find_if(m_loaded_resources_new.begin(), m_loaded_resources_new.end(), ResInformation::FindPredicate(i_handle));
				if (it == m_loaded_resources_new.end())
				{
					m_loaded_resources_new.emplace_back(hash, 1, ResInformation::State::Loaded, i_handle);
				}
				else
				{
					++it->m_use_count;
				}
			}

			template <typename ResType>
			void Unload(int i_handle)
			{
				auto it = std::find_if(m_loaded_resources_new.begin(), m_loaded_resources_new.end(), ResInformation::FindPredicate(i_handle));
				// <PANIC> Who creates this? It`s not ours; so we do nothing!
				if (it == m_loaded_resources_new.end())
				{
					assert(false && "It is not our Mesh");
					return;
				}

				--it->m_use_count;
				// the last one -> delete buffers
				if (it->m_use_count == 0)
				{
					Serialization::LoaderImpl<ResType>::UnloadResource(it->m_handle);
					m_loaded_resources_new.erase(it, m_loaded_resources_new.end());
				}
				// otherwise we do nothing
			}
		};


		// global instance for loading
		// TODO: decide in what format user should access objects like this
		GAMECORE_EXPORT extern LoadManager& g_load_manager;

	} // Resources

} // SDK

#endif