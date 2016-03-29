#ifndef __MATH_PLANE_H__
#define __MATH_PLANE_H__

#include "Vector.h"

namespace SDK
{

	namespace Math
	{
		template<class CoordinateType, class Enable = void>
		class Plane;

		template <typename CoordinateType>
		class Plane <CoordinateType, typename std::enable_if<std::is_floating_point<CoordinateType>::value>::type>
		{
		public:
			typedef Vector < CoordinateType, 3 > Vector3;
			typedef Vector < CoordinateType, 4 > Vector4;

		private:
			Vector3 normal;
			CoordinateType d;

		public:
			Plane()
			{
				normal = Vector3();
				d = CoordinateType(0);
			}

			Plane(const Plane& i_other)
			{
				normal = i_other.normal;
				d = i_other.d;
			}

			Plane(CoordinateType a, CoordinateType b, CoordinateType c, CoordinateType i_d)
				: normal(VectorConstructor<CoordinateType>::Construct(a, b, c))
				, d(i_d)
			{	}

			Plane(const Vector3& n, float i_d)
				: normal(n)
				, d(d)
			{	}

			Plane(const Vector3& rkPoint0, const Vector3& rkPoint1,
				const Vector3& rkPoint2)
			{
				redefine(rkPoint0, rkPoint1, rkPoint2);
			}

			Plane(const Vector3& i_normal, const Vector3& i_point)
			{
				redefine(i_normal, i_point);
			}

			inline void Inverse()
			{
				normal = -normal;
			}


			/** Redefine this plane based on 3 points. */
			void redefine(const Vector3& rkPoint0, const Vector3& rkPoint1,
				const Vector3& rkPoint2)
			{
				Vector3 edge_1 = rkPoint1 - rkPoint1;
				Vector3 edge_2 = rkPoint2 - rkPoint0;
				normal = edge_1.CrossProduct(edge_2);
				normal.Normalize();
				d = -normal.DotProduct(rkPoint0);
			}

			/** Redefine this plane based on a normal and a point. */
			void redefine(const Vector3& rkNormal, const Vector3& rkPoint)
			{
				normal = rkNormal;
				d = -rkNormal.dotProduct(rkPoint);
			}

			/*
			Represents side corresponding to plane
				no-side = on plane
				positive side = to normal direction
				negative side = -normal
				both-side = box
			*/
			enum class Side
			{
				NO_SIDE,
				POSITIVE_SIDE,
				NEGATIVE_SIDE,
				BOTH_SIDE
			};

			Side GetSide(const Vector3& i_point) const
			{
				auto dist = Distance(i_point);

				if (dist < CoordinateType(0))
					return Side::NEGATIVE_SIDE;

				if (dist > CoordinateType(0))
					return Side::POSITIVE_SIDE;

				return Side::NO_SIDE;
			}

			inline CoordinateType Distance(const Vector3& i_point) const
			{
				return normal.DotProduct(i_point) + d;
			}

			//! Normalize plane equation
			inline void Normalize()
			{
				CoordinateType length = normal.Length();
				if (length > CoordinateType(0))
				{
					CoordinateType inv_length = 1.0f / length;
					normal *= inv_length;
					d *= inv_length;
				}
			}

			/// Projects point onto this plane
			Vector3 ProjectVector(const Vector3& i_p) const
			{
				float dist = Distance(i_p);
				return i_p - dist*normal;
			}

			/// Comparison operator
			bool operator==(const Plane& rhs) const
			{
				return (rhs.d == d && rhs.normal == normal);
			}
			bool operator!=(const Plane& rhs) const
			{
				return (rhs.d != d || rhs.normal != normal);
			}
			

			Vector3 GetNormal() const
			{
				return normal;
			}

			CoordinateType GetD() const
			{
				return d;
			}
		};

	} // Math

} // SDK

#endif