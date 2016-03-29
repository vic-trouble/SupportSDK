#ifndef	__MATH_QUATERNION_H__
#define __MATH_QUATERNION_H__

#include "Vector.h"
#include "VectorConstructor.h"

#include "Matrix.h"

namespace SDK
{
	namespace Math
	{

		template<class CoordinateType, class Enable = void>
		class Quaternion;

		template <typename CoordinateType>
		class Quaternion <CoordinateType, typename std::enable_if<std::is_floating_point<CoordinateType>::value>::type>
		{
			union
			{
				CoordinateType m_data[4];
				struct {
					CoordinateType w, x, y, z;
				};
			};

		public:
			typedef Matrix<CoordinateType, 3, 3> Matrix3;
			typedef Vector<CoordinateType, 3> Vector3;
			typedef CoordinateType CT;
		public:
			static const CoordinateType msEpsilon;
			static const Quaternion ZERO;
			static const Quaternion IDENTITY;

		public:
			inline Quaternion();
			inline Quaternion(CoordinateType i_w, CoordinateType i_x, CoordinateType i_y, CoordinateType i_z);
			inline Quaternion(const Quaternion& i_other);
			inline Quaternion<CT>& operator = (const Quaternion<CT>& i_other);

			inline Quaternion(const Matrix<CoordinateType, 3, 3>& i_rotation_matrix)
			{
				this->FromRotationMatrix(i_rotation_matrix);
			}

			/// Construct a quaternion from an angle/axis
			inline Quaternion(const CoordinateType& i_radian_angle, const Vector<CoordinateType, 3>& i_axis)
			{
				this->FromAngleAxis(i_radian_angle, i_axis);
			}
			/// Construct a quaternion from 3 orthonormal local axes
			inline Quaternion(const Vector<CoordinateType, 3>& xaxis, const Vector<CoordinateType, 3>& yaxis, const Vector<CoordinateType, 3>& zaxis)
			{
				this->FromAxes(xaxis, yaxis, zaxis);
			}

			void FromRotationMatrix(const Matrix3& i_rot);
			void ToRotationMatrix(Matrix3& o_rot) const;

			/*
			Setups the quaternion using the supplied vector, and "roll" around
			that vector by the specified radians.
				The quaternion representing the rotation is
				  q = cos(A/2)+sin(A/2)*(x*i+y*j+z*k)
			*/
			void FromAngleAxis(const CoordinateType& i_radian_angle, const Vector3& i_axis);
			void ToAngleAxis(CoordinateType& oi_radian_angle, Vector3& o_axis) const;

			void FromAxes(const Vector3& xAxis, const Vector3& yAxis, const Vector3& zAxis);
			void ToAxes(Vector3& xAxis, Vector3& yAxis, Vector3& zAxis) const;

			inline void Set(CoordinateType w, CoordinateType x, CoordinateType y, CoordinateType z);

			CoordinateType DotProduct(const Quaternion<CT>& i_right) const;

			inline CoordinateType Length() const;
			inline CoordinateType LengthSq() const;
			inline void Normalize();
			inline void Conjugate();

			inline CoordinateType operator[] (size_t index) const { return m_data[index]; }
			inline CoordinateType& operator[] (size_t index) { return m_data[index]; }



			inline Quaternion<CT>& operator += (const Quaternion<CoordinateType>& i_other);
			inline Quaternion<CT> operator + (const Quaternion<CoordinateType>& i_other) const;

			inline Quaternion<CT>& operator -= (const Quaternion<CoordinateType>& i_other);
			inline Quaternion<CT> operator - (const Quaternion<CoordinateType>& i_other) const;

			Quaternion<CT> operator * (CoordinateType i_scalar) const;
			template <typename CT>
			friend Quaternion<CT> operator * (CoordinateType i_scalar,
				const Quaternion<CT>& i_right);

			inline Quaternion<CT>& operator *= (const Quaternion<CoordinateType>& i_other);
			inline Quaternion<CT> operator * (const Quaternion<CoordinateType>& i_other) const;

			inline Quaternion<CT> operator- () const;
			inline bool operator == (const Quaternion<CoordinateType>& rhs) const
			{
				return (rhs.x == x) && (rhs.y == y) &&
					(rhs.z == z) && (rhs.w == w);
			}
			inline bool operator != (const Quaternion<CoordinateType>& rhs) const
			{
				return !operator==(rhs);
			}

			//see Molecular engine http://molecularmusings.wordpress.com/2013/05/24/a-faster-quaternion-vector-multiplication/
			inline Vector<CoordinateType, 3> ApplyTo(const Vector<CoordinateType, 3>& i_vector) const;

			static Quaternion<CT> Slerp(CoordinateType t, const Quaternion<CT>& p, const Quaternion<CT>& q, bool shortest_path = false);
			/* Performs Normalised linear interpolation between two quaternions, and returns the result.
			nlerp ( 0.0f, A, B ) = A
			nlerp ( 1.0f, A, B ) = B
			@remarks
			Nlerp is faster than Slerp.
			Nlerp has the proprieties of being commutative (@see Slerp;
			commutativity is desired in certain places, like IK animation), and
			being torque-minimal (unless shortestPath=false). However, it's performing
			the interpolation at non-constant velocity; sometimes this is desired,
			sometimes it is not. Having a non-constant velocity can produce a more
			natural rotation feeling without the need of tweaking the weights; however
			if your scene relies on the timing of the rotation or assumes it will point
			at a specific angle at a specific weight value, Slerp is a better choice.
			*/
			static Quaternion<CT> Nlerp(CoordinateType t, const Quaternion<CT>& p, const Quaternion<CT>& q, bool shortest_path = false);
		};

	} // Math
} // SDK

#include "QuaternionImpl.h"

#endif