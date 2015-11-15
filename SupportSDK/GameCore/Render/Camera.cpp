#include "stdafx.h"

#include "Camera.h"

namespace SDK
{

	namespace Render
	{

		Camera::Camera()
			: m_need_recalc(true)
		{}

		void Camera::CalculateModelView() const
		{
			if (m_need_recalc == false)
				return;

			Matrix3 rot_mat;
			m_orientation.ToRotationMatrix(rot_mat);
			m_modelview_matrix.Identity();

			// rotation
			m_modelview_matrix[0][0] = rot_mat[0][0]; m_modelview_matrix[0][1] = rot_mat[0][1]; m_modelview_matrix[0][2] = rot_mat[0][2];
			m_modelview_matrix[1][0] = rot_mat[1][0]; m_modelview_matrix[1][1] = rot_mat[1][1]; m_modelview_matrix[1][2] = rot_mat[1][2];
			m_modelview_matrix[2][0] = rot_mat[2][0]; m_modelview_matrix[2][1] = rot_mat[2][1]; m_modelview_matrix[2][2] = rot_mat[2][2];
			// position
			m_modelview_matrix[0][3] = m_position[0]; m_modelview_matrix[1][3] = m_position[1]; m_modelview_matrix[2][3] = m_position[2];

			m_need_recalc = false;
		}

		Matrix4f Camera::GetModelViewMatrix() const
		{
			CalculateModelView();
			return m_modelview_matrix;
		}

		void ComputeLefUpForward(Vector3& o_left, Vector3& o_up, Vector3& o_forward, const Vector3& i_position, const Vector3& i_target)
		{
			// compute the forward vector
			o_forward = i_target - i_position;
			o_forward.Normalize();

			// compute temporal up vector based on the forward vector
			// watch out when look up/down at 90 degree
			// for example, forward vector is on the Y axis
			if (fabs(o_forward[0]) < Math::EPSILON && fabs(o_forward[2]) < Math::EPSILON)
			{
				// forward vector is pointing +Y axis
				if (o_forward[1] > 0)
					o_up = Vector3 { 0, 0, -1 };
				// forward vector is pointing -Y axis
				else
					o_up = Vector3 { 0, 0, 1 };
			}
			// in general, up vector is straight up
			else
			{
				o_up = Vector3 { 0, 1, 0 };
			}

			// compute the left vector
			o_left = o_up.CrossProduct(o_forward);  // cross product
			o_left.Normalize();

			// re-calculate the orthonormal up vector
			o_up = o_forward.CrossProduct(o_left);  // cross product
			o_up.Normalize();
		}

		void Camera::Calculate()
		{
			Vector3 left;
			Vector3 up;
			Vector3 forward;
			ComputeLefUpForward(left, up, forward, m_position, m_target);
			
			m_orientation.FromAxes(left, up, forward);

			m_need_recalc = true;
		}

		void Camera::Yaw(Radian angle)
		{
			// rotate around Y axis
			Vector3 axis = m_orientation.ApplyTo({ 0, 1, 0 });
			Rotate(axis, angle);
		}

		void Camera::Pitch(Radian angle)
		{
			// rotate around X axis
			Vector3 axis = m_orientation.ApplyTo({ 1, 0, 0 });
			Rotate(axis, angle);
		}

		void Camera::Roll(Radian angle)
		{
			// rotate around Z axis
			Vector3 axis = m_orientation.ApplyTo({ 0, 0, 1 });
			Rotate(axis, angle);
		}

		void Camera::Rotate(const Vector3& i_axis, Radian i_angle)
		{
			QuaternionF q;
			q.FromAngleAxis(i_angle, i_axis);
			Rotate(q);
		}

		void Camera::Rotate(const QuaternionF& i_q)
		{
			QuaternionF qnorm = i_q;
			qnorm.Normalize();
			m_orientation = qnorm * m_orientation;
			m_need_recalc = true;
		}

	} // Render

} // SDK