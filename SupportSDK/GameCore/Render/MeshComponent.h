#ifndef __GAMECORE_MESHCOMPONENT_H__
#define __GAMECORE_MESHCOMPONENT_H__

#include "Mesh.h"

#include "../Component.h"

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

		public:
			MeshComponent() {}
			MeshComponent(MeshHandler i_handler)
				: m_mesh(i_handler)
			{}			

			MeshHandler GetHandler() const { return m_mesh; }
		};

	} // Render

} // SDK

#endif