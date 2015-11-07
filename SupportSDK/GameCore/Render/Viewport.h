#ifndef	__GAMECORE_VIEWPORT_H__
#define __GAMECORE_VIEWPORT_H__

namespace SDK
{

	namespace Render
	{

		class Camera;
		class Frustum;

		class Viewport
		{
		public:
			Camera* mp_camera;
			Frustum* mp_frustum;
		};

	} // Render

} // SDK

#endif