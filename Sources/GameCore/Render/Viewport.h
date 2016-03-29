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
			Matrix4f m_projection_matrix;
			Matrix4f m_modelview_matrix;
			ProjectionType m_projection_type;
			
			Viewport()
				: m_projection_type(ProjectionType::Perspective)				
			{
				m_projection_matrix.Identity();
				m_modelview_matrix.Identity();
			}

			Viewport(ProjectionType i_projection_type, Matrix4f i_proj_mat, Matrix4f i_modelview)
				: m_projection_matrix(i_proj_mat)
				, m_modelview_matrix(i_modelview)
				, m_projection_type(i_projection_type)
			{}
		};

	} // Render

} // SDK

#endif