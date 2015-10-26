#ifndef __GAMECORE_RENDERSYSTEM__
#define __GAMECORE_RENDERSYSTEM__

#include "MeshComponent.h"

namespace SDK
{

	namespace Render
	{

		class RenderSystem
		{
		private:
			std::vector<MeshComponent> m_meshes;
		

		public:
			RenderSystem();

			void AddMesh(MeshComponent i_mesh);

			void Update(float i_elapsed_time);
			void Draw();
		};

	} // Render

} // SDK

#endif