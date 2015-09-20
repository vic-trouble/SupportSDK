#ifndef __MATH_MATRIXHELPERS_H__
#define __MATH_MATRIXHELPERS_H__

#include "Matrix.h"

namespace SDK
{

	namespace Math
	{

		template <typename CoordinateType, size_t Rows, size_t Collumns>
		class MatrixUtilities
		{
		public:
			static CoordinateType Determinant(const Matrix<CoordinateType, Rows, Collumns>& i_matrix);

			static Matrix<CoordinateType, Rows, Collumns> Inverse(const Matrix<CoordinateType, Rows, Collumns>& i_matrix);
		};

		template <typename CoordinateType>
		class MatrixUtilities < CoordinateType, 2, 2 >
		{
		public:
			static CoordinateType Determinant(const Matrix<CoordinateType, 2, 2>& i_matrix)
			{
				return i_matrix(0, 0)*i_matrix(1, 1) - i_matrix(0, 1)*i_matrix(1, 0);
			}

			static Matrix<CoordinateType, 2, 2> Inverse(const Matrix<CoordinateType, 2, 2>& i_m)
			{
				const CoordinateType det = Determinant(i_m);
				if (std::abs(det) <= std::numeric_limits<CoordinateType>::min())
					throw std::logic_error("Determinant can not be zero to calculate inverse");
				
				const CoordinateType coef = static_cast<CoordinateType>(1. / det);
				CoordinateType inv_matrix[] = { i_m(1,1)*coef, -i_m(0,1)*coef, -i_m(1,0)*coef, i_m(0,0)*coef };
				return Matrix<CoordinateType, 2, 2>(inv_matrix);
			}
		};

		template <typename CoordinateType>
		class MatrixUtilities < CoordinateType, 3, 3 >
		{
		public:
			static CoordinateType Determinant(const Matrix<CoordinateType, 3, 3>& i_m)
			{
				return i_m(0, 0)*(i_m(1, 1)*i_m(2, 2) - i_m(1, 2)*i_m(2, 1))
					- i_m(0, 1)*(i_m(1, 0)*i_m(2, 2) - i_m(1, 2)*i_m(2, 0))
					+ i_m(0, 2)*(i_m(1, 0)*i_m(2, 1) - i_m(1, 1)*i_m(2, 0));
			}

			static Matrix<CoordinateType, 3, 3> Inverse(const Matrix<CoordinateType, 3, 3>& i_m)
			{
				const CoordinateType det = Determinant(i_m);
				if (std::abs(det) <= std::numeric_limits<CoordinateType>::min())
					throw std::logic_error("Determinant can not be zero to calculate inverse");

				const CoordinateType coef = static_cast<CoordinateType>(1. / det);

				CoordinateType inv_matrix[] = { 
					(i_m(1, 1)*i_m(2, 2) - i_m(1, 2)*i_m(2, 1))*coef, -(i_m(0, 1)*i_m(2, 2) - i_m(0, 2)*i_m(2, 1))*coef, (i_m(0, 1)*i_m(1, 2) - i_m(0, 2)*i_m(1, 1))*coef,
					-(i_m(1, 0)*i_m(2, 2) - i_m(1, 2)*i_m(2, 0))*coef, (i_m(0, 0)*i_m(2, 2) - i_m(0, 2)*i_m(2, 0))*coef, -(i_m(0, 0)*i_m(1, 2) - i_m(0, 2)*i_m(1, 0))*coef,
					(i_m(1, 0)*i_m(2, 1) - i_m(1, 1)*i_m(2, 0))*coef, -(i_m(0, 0)*i_m(2, 1) - i_m(0, 1)*i_m(2, 0))*coef, (i_m(0, 0)*i_m(1, 1) - i_m(0, 1)*i_m(1, 0))*coef
				};

				return Matrix<CoordinateType, 3, 3>(inv_matrix);
			}
		};

		template <typename CoordinateType>
		class MatrixUtilities < CoordinateType, 4, 4 >
		{
		public:
		

		};

	} // Math

} // SDK

#endif