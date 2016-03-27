#ifndef __GAMECORE_MESHCOMPONENT_H__
#define __GAMECORE_MESHCOMPONENT_H__

#include "Mesh.h"

#include "../Component.h"
#include "../Entity.h"

namespace SDK
{

	namespace Render
	{
		class MeshSystem;
		
		class MeshComponent
		{
		public:
			ENTITY_DEFINITION(MeshSystem, "MeshComponent")

		private:
			MeshHandle m_mesh;

			EntityHandle m_entity;

		public:
			MeshComponent() {}
			MeshComponent(MeshHandle i_handler)
				: m_mesh(i_handler)
			{}			

			MeshHandle GetHandler() const { return m_mesh; }

			void SetEntity(EntityHandle i_entity) { m_entity = i_entity; }
			EntityHandle GetEntity() const { return m_entity; }
		};

	} // Render

} // SDK

#endif