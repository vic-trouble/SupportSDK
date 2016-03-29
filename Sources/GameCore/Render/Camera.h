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
			// Pre-calculated modelview matrix
			mutable Matrix4f m_modelview_matrix;
			mutable bool m_need_recalc;

			Vector3 m_position;
			Vector3 m_target;
			QuaternionF m_orientation;

		private:
			GAMECORE_EXPORT void Calculate();
			GAMECORE_EXPORT void CalculateModelView() const;
		public:
			Camera();

			// Matrix that will be applied to transform object coordinates
			//	to eye coordinates
			Matrix4f GetModelViewMatrix() const
			{
				CalculateModelView();
				return m_modelview_matrix;
			}

			void SetPosition(const Vector3& i_position) { m_position = i_position; Calculate(); }
			Vector3 GetPosition() const { return m_position; }

			void SetDirection(const Vector3& i_target) { m_target = i_target; Calculate(); }

			void LookAt(const Vector3& i_position, const Vector3& i_target)
			{
				m_position = i_position;
				m_target = i_target;
				Calculate();
			}

			/** Rolls the camera anticlockwise, around its local z axis.
			*/
			GAMECORE_EXPORT void Roll(Radian angle);

			/** Rotates the camera anticlockwise around it's local y axis.
			*/
			GAMECORE_EXPORT void Yaw(Radian angle);

			/** Pitches the camera up/down anticlockwise around it's local x axis.
			*/
			GAMECORE_EXPORT void Pitch(Radian angle);

			GAMECORE_EXPORT void Rotate(const Vector3& i_axis, Radian i_angle);
			GAMECORE_EXPORT void Rotate(const QuaternionF& i_q);
		};

	} // Render

} // SDK

#endif