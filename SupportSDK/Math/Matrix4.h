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
			typedef Quaternion<CoordinateType> ThisQuaternion;

		public:

			inline Matrix4()
				: Matrix()
			{}

			// Construct matrix from array which must be Rows*Collumns
			inline Matrix4(CoordinateType* ip_data)
				: Matrix(ip_data)
			{}

			inline Matrix4(const Matrix4& i_other)
				: Matrix(i_other)
			{}

			inline Matrix4(const Matrix<CoordinateType, 4, 4>& i_other)
				: Matrix(i_other)
			{}

			inline Matrix4(
				float _11, float _12, float _13, float _14,
				float _21, float _22, float _23, float _24,
				float _31, float _32, float _33, float _34,
				float _41, float _42, float _43, float _44
				)
			{
				m_data_[0][0] = _11; m_data_[0][1] = _12; m_data_[0][2] = _13; m_data_[0][3] = _14;
				m_data_[1][0] = _21; m_data_[1][1] = _22; m_data_[1][2] = _23; m_data_[1][3] = _24;
				m_data_[2][0] = _31; m_data_[2][1] = _32; m_data_[2][2] = _33; m_data_[2][3] = _34;
				m_data_[3][0] = _41; m_data_[3][1] = _42; m_data_[3][2] = _43; m_data_[3][3] = _44;
				
			}

			inline Matrix4 GetInverse() const;

			inline bool Decomposition(ThisVector3& position, ThisVector3& scale, ThisVector3& orientation) const;
			void Decomposition(ThisVector3& position, ThisVector3& scale, ThisQuaternion& orientation) const;

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

			inline void MakeTransform(const ThisVector3& position, const ThisVector3& scale, const ThisQuaternion& orientation);
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

		template <typename CoordinateType>
		void Matrix4<CoordinateType>::MakeTransform(const ThisVector3& position, const ThisVector3& scale, const ThisQuaternion& orientation)
		{
			// Ordering:
			//    1. Scale
			//    2. Rotate
			//    3. Translate
			typedef Matrix<CoordinateType, 3, 3> Matrix3;
			Matrix3 rot3x3;
			orientation.ToRotationMatrix(rot3x3);

			auto& m = m_data_;

			// Set up final matrix with scale, rotation and translation
			m[0][0] = scale[0] * rot3x3[0][0]; m[0][1] = scale[1] * rot3x3[0][1]; m[0][2] = scale[2] * rot3x3[0][2]; m[0][3] = position[0];
			m[1][0] = scale[0] * rot3x3[1][0]; m[1][1] = scale[1] * rot3x3[1][1]; m[1][2] = scale[2] * rot3x3[1][2]; m[1][3] = position[1];
			m[2][0] = scale[0] * rot3x3[2][0]; m[2][1] = scale[1] * rot3x3[2][1]; m[2][2] = scale[2] * rot3x3[2][2]; m[2][3] = position[2];

			// No projection term
			m[3][0] = 0; m[3][1] = 0; m[3][2] = 0; m[3][3] = 1;
		}

	} // Math

} // SDK

#endif