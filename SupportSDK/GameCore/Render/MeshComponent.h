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
			static constexpr int ID = static_cast<int>(ComponentClass::MeshClass);
			typedef MeshSystem ProcessorSystem;

		private:
			MeshHandler m_mesh;

			EntityHandle m_entity;

		public:
			MeshComponent() {}
			MeshComponent(MeshHandler i_handler)
				: m_mesh(i_handler)
			{}			

			MeshHandler GetHandler() const { return m_mesh; }

			void SetEntity(EntityHandle i_entity) { m_entity = i_entity; }
			EntityHandle GetEntity() const { return m_entity; }
		};

	} // Render

} // SDK

#endif