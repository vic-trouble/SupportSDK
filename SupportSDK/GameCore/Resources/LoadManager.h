#ifndef __GAMECORE_LOADMANAGER_H__
#define __GAMECORE_LOADMANAGER_H__

#include "../GameCoreAPI.h"

#include "ResourceInformation.h"

#include "../Render/Mesh.h"

namespace SDK
{
	
	namespace Resources
	{

		class LoadManager
		{
		private:
			typedef ResourceInformation<Render::Mesh> MeshResourceInfo;
			std::vector<MeshResourceInfo> m_loaded_resources;

		private:
			static Render::Mesh LoadObj(const std::string& i_file_name, Render::BufferUsageFormat i_vertices_usage, Render::BufferUsageFormat i_indices_usage);

		public:
			// TODO: descriptions
			// TODO: different loading modes: return loaded; reload; create new
			// load; if loaded - return loaded

			// for now: i_file_name - relative path which depends on executable
			Render::Mesh	GAMECORE_EXPORT LoadMesh(const std::string& i_file_name, Render::BufferUsageFormat i_vertices_usage, Render::BufferUsageFormat i_indices_usage);
			void			GAMECORE_EXPORT ReleaseMesh(const Render::Mesh& i_mesh);
		};


		// global instance for loading
		// TODO: decide in what format user should access objects like this
		GAMECORE_EXPORT extern LoadManager& g_load_manager;

	} // Resources

} // SDK

#endif