#ifndef __GAMECORE_FRUSTUM_H__
#define __GAMECORE_FRUSTUM_H__

#include "../GameCoreAPI.h"

#include "../Render/RenderTypes.h"

namespace SDK
{

	namespace Render
	{		

		class Frustum
		{
		public:
			// TODO: make type Radian
			typedef float Radian;
		private:
			ProjectionType m_projection_type;

			Radian m_fov;

			real m_near_dist;
			real m_far_dist;
			real m_aspect;

			PlaneF m_frustum_planes[6];

			real m_left;
			real m_right;
			real m_bottom;
			real m_top;

			Matrix4f m_projection_matrix;

			bool m_valid;

		private:
			GAMECORE_EXPORT void InvalidateFrustum();

			void CalculateOrtho();
			void CalculatePerspective();

		public:
			Frustum();

			ProjectionType GetProjectionType() const { return m_projection_type; }
			void SetProjectionType(ProjectionType i_projection_type)
			{
				m_projection_type = i_projection_type;
				m_valid = false;
				InvalidateFrustum();
			}

			void SetFOV(const Radian& i_fov)
			{
				m_fov = i_fov;
				m_valid = false;
				InvalidateFrustum();
			}
			const Radian& GetFOV() const { return m_fov; }

			void SetNearClipDistance(real i_near_dist)
			{
				m_near_dist = i_near_dist;
				m_valid = false;
				InvalidateFrustum();
			}
			real GetNearClipDistance() const { return m_near_dist; }

			void SetFarClipDistance(real i_far_dist)
			{
				m_far_dist = i_far_dist;
				m_valid = false;
				InvalidateFrustum();
			}
			real GetFarClipDistance() const { return m_far_dist; }

			void SetAspectRatio(real i_aspect)
			{
				m_aspect = i_aspect;
				m_valid = false;
				InvalidateFrustum();
			}
			real GetAspectRatio() const { return m_aspect; }

			void SetFrustum(Radian i_fov, real i_aspect, real i_near, real i_far)
			{
				m_near_dist = i_near;
				m_far_dist = i_far;
				m_fov = i_fov;
				m_aspect = i_aspect;
				m_valid = false;
				InvalidateFrustum();
			}
			void SetFrustumExtents(real i_left, real i_right, real i_bottom, real i_top, real i_near, real i_far)
			{
				m_left = i_left;
				m_right = i_right;
				m_bottom = i_bottom;
				m_top = i_top;
				m_near_dist = i_near;
				m_far_dist = i_far;
				m_valid = false;
				InvalidateFrustum();
			}

			Matrix4f GetProjectionMatrix() const { return m_projection_matrix; }

			// in_abbs
			//	return visible (or mark)
			void Cull() const;
		};

	} // Render

} // SDK

#endif