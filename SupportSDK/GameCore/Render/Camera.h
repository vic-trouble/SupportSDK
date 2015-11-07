#ifndef	__GAMECORE_CAMERA_H__
#define __GAMECORE_CAMERA_H__

#include "../GameCoreAPI.h"

namespace SDK
{
	
	namespace Render
	{

		class Camera
		{
		public:
			Matrix4f m_modelview_matrix;
		public:
			// Matrix that will be applied to transform object coordinates
			//	to eye coordinates
			Matrix4f GetModelViewMatrix() const;
		};

	} // Render

} // SDK

#endif