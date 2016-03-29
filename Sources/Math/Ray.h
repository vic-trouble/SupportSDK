#ifndef	__MATH_RAY_H__
#define __MATH_RAY_H__

#include "Vector.h"
#include "VectorConstructor.h"

#include "Plane.h"

namespace SDK
{
	namespace Math
	{

		template<class CoordinateType, class Enable = void>
		class Ray;

		template <typename CoordinateType>
		class Ray < CoordinateType, typename std::enable_if<std::is_floating_point<CoordinateType>::value>::type >
		{
		public:
			typedef Vector<CoordinateType, 3> Vector3;

		private:
			Vector3 m_origin;
			Vector3 m_direction;

		public:
			Ray() : m_origin(), m_direction(VectorConstructor<CoordinateType>::Construct(0, 0, 1)) {}
			Ray(const Vector3& origin, const Vector3& direction)
				: m_origin(origin), m_direction(direction) {}

			/** Sets the origin of the ray. */
			void setOrigin(const Vector3& origin) { m_origin = origin; }
			/** Gets the origin of the ray. */
			const Vector3& getOrigin(void) const { return m_origin; }

			/** Sets the direction of the ray. */
			void setDirection(const Vector3& dir) { m_direction = dir; }
			/** Gets the direction of the ray. */
			const Vector3& getDirection(void) const { return m_direction; }

			/** Gets the position of a point t units along the ray. */
			Vector3 getPoint(CoordinateType t) const {
				return Vector3(m_origin + (m_direction * t));
			}

			/** Gets the position of a point t units along the ray. */
			Vector3 operator*(CoordinateType t) const {
				return getPoint(t);
			}

			typedef std::pair<bool, CoordinateType> Intersection;
			Intersection Itersects(const Plane<CoordinateType>& i_plane) const
			{
				Intersection inter(false, CoordinateType(0.0));

				CoordinateType denom = i_plane.GetNormal().DotProduct(m_direction);

				if (std::abs(denom) < std::numeric_limits<CoordinateType>::min())
					return inter;

				inter.first = true;
				inter.second = (-(i_plane.GetD() + i_plane.GetNormal().DotProduct(m_origin))) / denom;

				return inter;
			}
		};

	} // Math
} // SDK

#endif