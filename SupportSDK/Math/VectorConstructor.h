#ifndef __VECTORCONSTRUCTOR_H__
#define __VECTORCONSTRUCTOR_H__

#include "Vector.h"

namespace SDK
	{

	namespace Math
		{

		template <typename CoordinateType>
		class VectorConstructor
			{
			public:
				static Vector<CoordinateType, 1> Construct (const CoordinateType& a)
					{
					Vector<CoordinateType, 2> vec;
					vec[0] = a;
					return vec;
					}

				static Vector<CoordinateType, 2> Construct (const CoordinateType& a, const CoordinateType& b)
					{
					Vector<CoordinateType, 2> vec;
					vec[0] = a; vec[1] = b;
					return vec;
					}

				static Vector<CoordinateType, 3> Construct (const CoordinateType& a, const CoordinateType& b, const CoordinateType& c)
					{
					Vector<CoordinateType, 3> vec;
					vec[0] = a; vec[1] = b; vec[2] = c;
					return vec;
					}

				static Vector<CoordinateType, 4> Construct (const CoordinateType& a, const CoordinateType& b, const CoordinateType& c, const CoordinateType& d)
					{
					Vector<CoordinateType, 3> vec;
					vec[0] = a; vec[1] = b; vec[2] = c; vec[3] = d;
					return vec;
					}
			};

		} // Math

	} // SDK

#endif