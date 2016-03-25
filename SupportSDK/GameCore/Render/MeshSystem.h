#ifndef	__GAMECORE_MESHSYSTEM_H__
#define __GAMECORE_MESHSYSTEM_H__

#include "../GameCoreAPI.h"

#include "System.h"
#include "Render/Material.h"
#include "Render/Mesh.h"
#include "Render/MeshComponent.h"

#include "GenericHandlesDynamicArray.h"

#include "../PropertyElement.h"

namespace SDK
{
	struct MeshInformation;
	namespace Resources {
		namespace Serialization {

			template <typename ResType>
			struct Definition;
			template <>
			struct Definition <Render::Mesh>
			{
				typedef MeshInformation InfoType;
				typedef Render::MeshHandler HandleType;
			};

			template <typename ResInfo>
			struct LoaderImpl;

		}// Serialization
	} // Resources

	namespace Render
	{
		class MeshSystem : public System
		{
		private:
			friend struct Resources::Serialization::LoaderImpl<Mesh>;

		private:
			// raw resources - not processed if they not used with MeshComponent
			std::vector<Mesh> m_meshes;
			std::vector<MeshHandler> m_handlers;
			std::vector<MeshComponent> m_instances;
			std::vector<MeshComponentHandler> m_component_handlers;

		public:
			MeshSystem();
			virtual ~MeshSystem();

			// System
			virtual void Update(float i_elapsed_time) override;
			virtual void SubmitDrawCommands() override;

			virtual bool Requires(Action i_aciton) const { return i_aciton == Action::SubmitDrawCommands || i_aciton == Action::Update; }

			// Own
			GAMECORE_EXPORT MeshHandler Load(const std::string& i_name, const std::string& i_path, BufferUsageFormat i_vertices_usage, BufferUsageFormat i_indices_usage);
			GAMECORE_EXPORT void Unload(MeshHandler i_handler);
			GAMECORE_EXPORT void Unload(const std::string& i_file_name) { throw std::exception("Not realized"); }
			
			GAMECORE_EXPORT MeshComponentHandler CreateInstance(MeshHandler i_handler);
			GAMECORE_EXPORT MeshComponentHandler CreateInstance(const std::string& i_file_name) { throw std::exception("Not realized"); }

			// TODO: Custom mesh
			//MeshHandler Create(const std::string& i_name, Mesh i_mesh);

			GAMECORE_EXPORT void AddMaterialTo(MeshComponentHandler i_component, MaterialHandle i_material);

			/////////////////////////////////////////////////////////
			
			// if you want to change something - use this method. On one frame it is guaranteed that 
			//	pointer will be valid if not nullptr
			GAMECORE_EXPORT MeshComponent* AccessComponent(MeshComponentHandler i_handler);
			GAMECORE_EXPORT void RemoveInstance(MeshComponentHandler i_handler);			

			void Initialize();
			void Release();
			void Load(const PropertyElement& i_resource_element);

		// Extension for entity manager
		public:
			GAMECORE_EXPORT static MeshComponent* Get(int i_in_system_id, int i_in_system_generation);
			GAMECORE_EXPORT static void Remove(int i_in_system_id, int i_in_system_generation);
		};

		// TODO: global object: decide in what format user should access objects like this
		GAMECORE_EXPORT extern MeshSystem& g_mesh_system;

	} // Render

} // SDK

#endif