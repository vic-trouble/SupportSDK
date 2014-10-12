#ifndef __MATHVECTORUTILITIES_H__
#define __MATHVECTORUTILITIES_H__

#include "Vector.h"

namespace SDK
	{

	namespace Math
		{

		template <typename CoordinateType, size_t Dimension>
		struct VectorUtilities
			{

			static Vector<CoordinateType, Dimension> Projection (const Vector<CoordinateType, Dimension>& i_p, const Vector<CoordinateType, Dimension>& i_q)
				{
				// proj = (DotProduct(p, q)/Length(Q)^2)*Q
				// For better performance for thre dimension space
				// 1. Dot product this * i_target
				CoordinateType dot_product = i_p.DotProduct(i_q);
				// 2. Dot product result / target`s length^2
				dot_product /= i_q.LengthSq();
				// 3. i_target * dot_product
				Vector<CoordinateType, Dimension> projection = i_q;
				projection *= dot_product;

				return projection;
				}

			/// perpendicular component of Pwith respect to Q
			static Vector<CoordinateType, Dimension> PerpendicularComponent (const Vector<CoordinateType, Dimension>& i_p, const Vector<CoordinateType, Dimension>& i_q)
				{
				auto perp = i_p;
				perp -= Projection(i_p, i_q);
				return perp;
				}

			};

		} // Math

	} // SDK

#endif