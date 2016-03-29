#ifndef __MATHCROSSPRODUCTCALCULATOR_H__
#define __MATHCROSSPRODUCTCALCULATOR_H__

namespace SDK
	{

	namespace Math
		{

		template <typename CoordinateType, size_t Dimension>
		struct CrossProductCalculator
			{
			static Vector<CoordinateType, Dimension> Calculate(const Vector<CoordinateType, Dimension>& i_first, const Vector<CoordinateType, Dimension>& i_second);
			};

		template <typename CoordinateType>
		struct CrossProductCalculator<CoordinateType, 2>
			{
			/*
			For not 3D space cross product is not well defined.
			Due to mathworld (http://mathworld.wolfram.com/CrossProduct.html)
			CrossProduct = Ux*Vy - Uy*Vx;
			*/
			static Vector<CoordinateType, 2> Calculate(const Vector<CoordinateType, 2>& i_first, const Vector<CoordinateType, 2>& i_second)
				{
				Vector<CoordinateType, 2> cross_product;

				cross_product[0] = i_first[0]*i_second[1] - i_first[1]*i_second[0];
				cross_product[1] = 0;

				return cross_product;
				}
			};

		template <typename CoordinateType>
		struct CrossProductCalculator<CoordinateType, 3>
			{
			static Vector<CoordinateType, 3> Calculate(const Vector<CoordinateType, 3>& i_first, const Vector<CoordinateType, 3>& i_second)
				{
				Vector<CoordinateType, 3> cross_product;

				cross_product[0] = i_first[1]*i_second[2] - i_first[2]*i_second[1];
				cross_product[1] = i_first[2]*i_second[0] - i_first[0]*i_second[2];
				cross_product[2] = i_first[0]*i_second[1] - i_first[1]*i_second[0];

				return cross_product;
				}
			};

		} // Math

	} // SDK

#endif