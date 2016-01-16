#include "stdafx.h"

#include "Frustum.h"

namespace SDK
{

	namespace Render
	{

		const real PI = 3.1419f;
		const real DEG_TO_RAD = PI / real(180);

		Frustum::Frustum()
			: m_left(real(-0.5))
			, m_right(real(0.5))
			, m_bottom(real(-0.5))
			, m_top(real(0.5))
			, m_aspect(real(0.667))
			, m_near_dist(real(1))
			, m_far_dist(real(1000))
			, m_fov(45 * DEG_TO_RAD)
			, m_projection_type(ProjectionType::Perspective)
			, m_valid(false)
		{
			InvalidateFrustum();
		}

		void Frustum::CalculateOrtho()
		{
			m_projection_matrix.Identity();
			m_projection_matrix(0) = 2 / (m_right - m_left);
			m_projection_matrix(3) = -(m_right + m_left) / (m_right - m_left);
			m_projection_matrix(5) = 2 / (m_top - m_bottom);
			m_projection_matrix(7) = -(m_top + m_bottom) / (m_top - m_bottom);
			m_projection_matrix(10) = -2 / (m_far_dist - m_near_dist);
			m_projection_matrix(11) = -(m_far_dist + m_near_dist) / (m_far_dist - m_near_dist);
		}

		void Frustum::CalculatePerspective()
		{
			// near and far are up to date, recalculate other parameters
			const real tangent = tanf(m_fov / 2);   // tangent of half fovY
			const real height = m_near_dist * tangent;           // half height of near plane
			const real width = height * m_aspect;       // half width of near plane

			m_left = -width;
			m_right = width;
			m_bottom = -height;
			m_top = height;

			m_projection_matrix.Identity();
			m_projection_matrix(0) = 2 * m_near_dist / (m_right - m_left);
			m_projection_matrix(2) = (m_right + m_left) / (m_right - m_left);
			m_projection_matrix(5) = 2 * m_near_dist / (m_top - m_bottom);
			m_projection_matrix(6) = (m_top + m_bottom) / (m_top - m_bottom);
			m_projection_matrix(10) = -(m_far_dist + m_near_dist) / (m_far_dist - m_near_dist);
			m_projection_matrix(11) = -(2 * m_far_dist * m_near_dist) / (m_far_dist - m_near_dist);
			m_projection_matrix(14) = -1;
			m_projection_matrix(15) = 0;
		}

		void Frustum::InvalidateFrustum()
		{
			if (m_valid)
				return;

			if (m_projection_type == ProjectionType::Perspective)
				CalculatePerspective();
			else
				CalculateOrtho();
			
			m_valid = true;
		}

		void Frustum::Cull() const
		{

		}

	} // Render

} // SDK