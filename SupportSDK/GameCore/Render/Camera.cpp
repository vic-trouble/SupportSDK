#include "stdafx.h"

#include "Camera.h"

namespace SDK
{

	namespace Render
	{

		Matrix4f Camera::GetModelViewMatrix() const
		{
			return m_modelview_matrix;
		}

	} // Render

} // SDK