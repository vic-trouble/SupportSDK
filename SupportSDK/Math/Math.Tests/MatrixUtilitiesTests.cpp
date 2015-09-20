#include "stdafx.h"

#include <Matrix.h>
#include <MatrixUtilities.h>
using namespace SDK::Math;

namespace
{
	template <typename CoordinateType, size_t Rows, size_t Collumns>
	std::ostream& operator << (std::ostream& o_stream, const Matrix<CoordinateType, Rows, Collumns>& i_matrix)
	{
		for (size_t i = 0; i < Rows; ++i)
		{
			for (size_t j = 0; j < Collumns; ++j)
				o_stream << i_matrix(i, j) << " ";
			o_stream << std::endl;
		}
		return o_stream;
	}
} // namespace

TEST(MatrixUtilitiesTests, Determinant_2x2)
{
	float m[] = { 1., 2., 3., 4. };
	Matrix<float, 2, 2> matrix(m);
	auto res = MatrixUtilities<float, 2, 2>::Determinant(matrix);

	EXPECT_FLOAT_EQ(-2.f, res);
}

TEST(MatrixUtilitiesTests, Inverse_2x2)
{
	float m[] = { 1., 2., 3., 4. };
	Matrix<float, 2, 2> matrix(m);
	auto res = MatrixUtilities<float, 2, 2>::Inverse(matrix);

	EXPECT_FLOAT_EQ(-2.f, res(0, 0));
	EXPECT_FLOAT_EQ(1.f, res(0, 1));
	EXPECT_FLOAT_EQ(1.5f, res(1, 0));
	EXPECT_FLOAT_EQ(-.5f, res(1, 1));
}

TEST(MatrixUtilitiesTests, Determinant_3x3)
{
	float m[] = { 1.f, 2.2f, 3.3f, 4.4f, 5.5f, 6.6f, 7.7f, 8.8f, 9.9f };
	Matrix<float, 3, 3> matrix(m);
	auto res = MatrixUtilities<float, 3, 3>::Determinant(matrix);

	EXPECT_NEAR(0.363f, res, 0.00001);
}

TEST(MatrixUtilitiesTests, Inverse_3x3)
{
	double m[] = { 2., 3., 8., 6., 0., -3., -1., 3., 2. };
	Matrix<double, 3, 3> matrix(m);

	auto inverse = MatrixUtilities<double, 3, 3>::Inverse(matrix);
	inverse *= 45.;
	EXPECT_DOUBLE_EQ(3., inverse(0, 0));
	EXPECT_DOUBLE_EQ(6., inverse(0, 1));
	EXPECT_DOUBLE_EQ(-3., inverse(0, 2));
	EXPECT_DOUBLE_EQ(-3., inverse(1, 0));
	EXPECT_DOUBLE_EQ(4., inverse(1, 1));
	EXPECT_DOUBLE_EQ(18., inverse(1, 2));
	EXPECT_DOUBLE_EQ(6., inverse(2, 0));
	EXPECT_DOUBLE_EQ(-3., inverse(2, 1));
	EXPECT_DOUBLE_EQ(-6., inverse(2, 2));
}