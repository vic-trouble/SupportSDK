#ifndef __GAMECORE_MESHCOMPONENT_H__
#define __GAMECORE_MESHCOMPONENT_H__

#include "Mesh.h"

namespace SDK
{

	namespace Render
	{

		class MeshComponent
		{
		private:
			Mesh m_mesh_data;

		public:
			const Mesh& GetMesh() const { return m_mesh_data; }
			Mesh& GetMeshNonConst() { return m_mesh_data; }
		};

	} // Render

} // SDK

#endif