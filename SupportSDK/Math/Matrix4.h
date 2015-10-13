#ifndef __MATH_MATRIX4_H__
#define __MATH_MATRIX4_H__

#include "Matrix.h"
#include "MatrixUtilities.h"
#include "Vector.h"
#include "VectorConstructor.h"

namespace SDK
{

	namespace Math
	{
		
		template <typename CoordinateType>
		class Matrix4 : public Matrix < CoordinateType, 4, 4 >
		{
		public:
			typedef Vector<CoordinateType, 3> ThisVector3;

		public:

			inline ThisMatrix GetInverse() const;

			inline bool Decomposition(Vector3& position, Vector3& scale, Vector3& orientation) const;
			void Decomposition(Vector3& position, Vector3& scale, Quaternion& orientation) const;

			// Create Rotation/Translation/Scale matrix from this Matrix and input parameters
			inline static ThisMatrix MakeTranslation(const ThisVector3& i_translation_vector);
			inline static ThisMatrix MakeRotation(const ThisVector3& i_axis, CoordinateType i_angle_radians);
			inline static ThisMatrix MakeScale(const ThisVector3& i_scale_vector);

			// Makes translation matrix for this matrix
			inline void CreateTranslation(const ThisVector3& i_vector);
			// Makes rotation matrix for this matrix
			inline void CreateRotation(const ThisVector3& i_axis, CoordinateType i_angle_radians);
			// Makes scale matrix for this matrix
			inline void CreateScale(const ThisVector3& i_vector);

			inline ThisVector3 GetTranslationVector() const;
			inline void SetTranslationVector(const ThisVector3 & vector);

			inline ThisVector3 GetScaleVector() const;
		};

		template <typename CoordinateType>
		Matrix4<CoordinateType> Matrix4<CoordinateType>::GetInverse() const
		{
			return MatrixUtilities<CoordinateType, 4, 4>::Inverse();
		}

		template <typename CoordinateType>
		Vector<CoordinateType, 3> Matrix4<CoordinateType>::GetScaleVector() const
		{
			Vector<CoordinateType, 3> x_axis = VectorConstructor<CoordinateType>::Construct(m_data_[0][0], m_data_[0][1], m_data_[0][2]);
			Vector<CoordinateType, 3> y_axis = VectorConstructor<CoordinateType>::Construct(m_data_[1][0], m_data_[1][1], m_data_[1][2]);
			Vector<CoordinateType, 3> z_axis = VectorConstructor<CoordinateType>::Construct(m_data_[2][0], m_data_[2][1], m_data_[2][2]);
			return VectorConstructor<CoordinateType>::Construct(x_axis.Length(), y_axis.Length(), z_axis.Length());
		}

		template <typename CoordinateType>
		Vector<CoordinateType, 3> Matrix4<CoordinateType>::GetTranslationVector() const
		{
			return VectorConstructor<CoordinateType>::Construct(m_data_[0][3], m_data_[1][3], m_data_[2][3]);
		}

		template <typename CoordinateType>
		void Matrix4<CoordinateType>::SetTranslationVector(const Vector<CoordinateType, 3>& i_vector)
		{
			/*
			-	-	-	x
			-	-	-	y
			-	-	-	z
			-	-	-	-
			*/
			m_data_[0][3] = i_vector[0];
			m_data_[1][3] = i_vector[1];
			m_data_[2][3] = i_vector[2];
		}

		template <typename CoordinateType>
		void Matrix4<CoordinateType>::CreateRotation(const ThisVector3& i_axis, CoordinateType i_angle_radians)
		{
			 // TODO: wrappers for cos/sin
			CoordinateType cosA = std::cos(i_angle_radians);
			CoordinateType sinA = std::sin(i_angle_radians);

			Identity();

			_data[0][0] = cosA + (1 - cosA) * i_axis[0] * i_axis[0];
			_data[0][1] = (1 - cosA) * i_axis[0] * i_axis[1] - i_axis[2] * sinA;
			_data[0][2] = (1 - cosA) * i_axis[0] * i_axis[2] + i_axis[1] * sinA;


			_data[1][0] = (1 - cosA) * i_axis[0] * i_axis[1] + i_axis[2] * sinA;
			_data[1][1] = cosA + (1 - cosA) * i_axis[1] * i_axis[1];
			_data[1][2] = (1 - cosA) * i_axis[1] * i_axis[2] - i_axis[0] * sinA;


			_data[2][0] = (1 - cosA) * i_axis[0] * i_axis[2] - i_axis[1] * sinA;
			_data[2][1] = (1 - cosA) * i_axis[1] * i_axis[2] + i_axis[0] * sinA;
			_data[2][2] = cosA + (1 - cosA) * i_axis[2] * i_axis[2];
		}

		template <typename CoordinateType>
		void Matrix4<CoordinateType>::CreateScale(const ThisVector3& i_vector)
		{
			Identity();
			/*
				x	0	0	0
				0	y	0	0
				0	0	z	0
				0	0	0	1
			*/
			m_data_[0][0] = i_vector[0];
			m_data_[1][1] = i_vector[1];
			m_data_[2][2] = i_vector[2];
		}

		template <typename CoordinateType>
		void Matrix4<CoordinateType>::CreateTranslation(const ThisVector3& i_vector)
		{
			Identity();
			SetTranslationVector(i_vector);
		}

	} // Math

} // SDK

#endif