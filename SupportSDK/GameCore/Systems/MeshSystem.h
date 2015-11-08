#ifndef	__GAMECORE_MESHSYSTEM_H__
#define __GAMECORE_MESHSYSTEM_H__

#include "../GameCoreAPI.h"

#include "../System.h"
#include "../Render/Mesh.h"

namespace SDK
{
	
	namespace Resources { namespace Serialization{

		template <typename ResInfo>
		struct LoaderImpl;

		} // Serialization
	} // Resources

	namespace Render
	{
		class MeshSystem : public System		
		{
		private:
			friend struct Resources::Serialization::LoaderImpl<Mesh>;

		private:
			std::vector<Mesh> m_meshes;
			std::vector<MeshHandler> m_handlers;

			static int CreateNewHandler();
			static void RemoveHandler(int i_handler);
			static void UnloadResource(int i_handler);

		public:
			MeshSystem();
			virtual ~MeshSystem();

			// System
			virtual void Update(float i_elapsed_time) override;
			virtual void SubmitDrawCommands() override;

			virtual bool Requires(Action i_aciton) const { return i_aciton == Action::SubmitDrawCommands || i_aciton == Action::Update; }

			// Own
			GAMECORE_EXPORT MeshHandler Load(const std::string& i_file_name, Render::BufferUsageFormat i_vertices_usage, Render::BufferUsageFormat i_indices_usage);
			GAMECORE_EXPORT void Unload(MeshHandler i_handler);
			void Register(const std::string& i_name, Mesh i_mesh);
		};

		// TODO: decide in what format user should access objects like this
		GAMECORE_EXPORT extern MeshSystem& g_mesh_system;

	} // Render

} // SDK

#endif