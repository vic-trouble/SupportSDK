#ifndef __MATH_UTILITIES_H__
#define __MATH_UTILITIES_H__

#include "Line.h"
#include "Vector.h"
#include "VectorUtilities.h"

namespace SDK
{

	namespace Math
	{

		template <typename CoordinateType, size_t Dimension>
		CoordinateType DistanceSq(const Vector<CoordinateType, Dimension>& i_point, const Line<CoordinateType, Dimension>& i_line)
		{
			auto q_s = i_point - i_line.From();
			auto proj = VectorUtilities<CoordinateType, Dimension>::Projection(q_s, i_line.Direction());
			auto distance = q_s.LengthSq() - proj.LengthSq();
			return distance;
		}

		template <typename CoordinateType, size_t Dimension>
		CoordinateType Distance(const Vector<CoordinateType, Dimension>& i_point, const Line<CoordinateType, Dimension>& i_line)
		{
			return sqrt(DistanceSq(i_point, i_line));
		}

	} // Math

} // SDK

#endif