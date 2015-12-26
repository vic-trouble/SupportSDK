#include "stdafx.h"
#include <Quaternion.h>
#include <Matrix.h>
#include <Matrix4.h>

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

TEST(MatrixTests, ConstructMatrix_ShouldBeZero)
{
	Matrix<double, 3, 3> matrix;
	for (size_t i = 0; i < Matrix<double, 3, 3>::RowsNumber; ++i)
	{
		for (size_t j = 0; j < Matrix<double, 3, 3>::CollumnsNumber; ++j)
			EXPECT_EQ(0, matrix(i,j));

	}
}

TEST(MatrixTests, AccessTest)
{
	double m[] = { 2., 3., 4., 5. };
	Matrix<double, 2, 2> matrix(m);

	EXPECT_EQ(matrix(0), matrix[0][0]);
	EXPECT_EQ(matrix(1), matrix[0][1]);
	EXPECT_EQ(matrix(2), matrix[1][0]);
	EXPECT_EQ(matrix(3), matrix[1][1]);
}

#if _MSC_VER >= 1800
TEST(MatrixTests, ConstructVariadicVector_ThrowIfInitListSizeBiggerThanMatrixDimensions)
{
	bool throwed = false;
	try
	{
		Matrix<double, 3, 3> matrix { 0., 1., 3.14, 2.15 };
	}
	catch (std::logic_error&)
	{
		throwed = true;
	}
	EXPECT_TRUE(throwed);
}

TEST(MatrixTests, ConstructMatrixWithInitializerList)
{
	Matrix<double, 2, 2> matrix{ 0., 1., 2., 3.};
	EXPECT_EQ(0., matrix(0, 0));
	EXPECT_EQ(1., matrix(0, 1));
	EXPECT_EQ(2., matrix(1, 0));
	EXPECT_EQ(3., matrix(1, 1));
}
#endif

TEST(MatrixTests, IdentityTest)
{
	double m_0[] = { 1., 2., 3., 4. };
	Matrix<double, 2, 2> matrix(m_0);
	matrix.Identity();
	EXPECT_EQ(1., matrix(0, 0));
	EXPECT_EQ(0., matrix(0, 1));
	EXPECT_EQ(0., matrix(1, 0));
	EXPECT_EQ(1., matrix(1, 1));
	EXPECT_TRUE(matrix.IsIdentity());	
}

TEST(MatrixTests, IdentityTest_False)
{
	double m_0[] = { 1., 2., 3., 4. };
	Matrix<double, 2, 2> matrix(m_0);	
	EXPECT_FALSE(matrix.IsIdentity());
}

TEST(MatrixTests, AddMatrices)
{
	double m_0[] = { 1., 2., 3., 4. };
	double m_1[] = { 2., 3., 4., 5. };

	Matrix<double, 2, 2> matrix_0(m_0);
	Matrix<double, 2, 2> matrix_1(m_1);

	auto result = matrix_0 + matrix_1;
	EXPECT_EQ(3., result(0, 0));
	EXPECT_EQ(5., result(0, 1));
	EXPECT_EQ(7., result(1, 0));
	EXPECT_EQ(9., result(1, 1));
}

TEST(MatrixTests, SubstituteMatrices)
{
	double m_0[] = { 1., 2., 3., 4. };
	double m_1[] = { 7., 9., 10., 5. };

	Matrix<double, 2, 2> matrix_0(m_0);
	Matrix<double, 2, 2> matrix_1(m_1);

	auto result = matrix_1 - matrix_0;
	EXPECT_EQ(6., result(0, 0));
	EXPECT_EQ(7., result(0, 1));
	EXPECT_EQ(7., result(1, 0));
	EXPECT_EQ(1., result(1, 1));
}

TEST(MatrixTests, EqualityTest_Equal)
{
	double m_0[] = { 1., 2., 3., 4. };
	double m_1[] = { 1., 2., 3., 4. };

	Matrix<double, 2, 2> matrix_0(m_0);
	Matrix<double, 2, 2> matrix_1(m_1);

	bool equal_test = matrix_0 == matrix_1;
	bool nequal_test = matrix_0 != matrix_1;

	EXPECT_TRUE(equal_test);
	EXPECT_FALSE(nequal_test);
}

TEST(MatrixTests, EqualityTest_NotEqual)
{
	double m_0[] = { 1., 2., 3., 4. };
	double m_1[] = { 1., 2., 3., 5. };

	Matrix<double, 2, 2> matrix_0(m_0);
	Matrix<double, 2, 2> matrix_1(m_1);

	bool equal_test = matrix_0 == matrix_1;
	bool nequal_test = matrix_0 != matrix_1;

	EXPECT_TRUE(nequal_test);
	EXPECT_FALSE(equal_test);
}

TEST(MatrixTests, MultiplyOnMatrixTest)
{
	double m_0[] = { 1., 2., 3., 4., 5., 6., 7., 8., 9. };
	double m_1[] = { 1., 2., 3., 4., 5., 6. };

	Matrix<double, 3, 3> matrix_0(m_0);
	Matrix<double, 3, 2> matrix_1(m_1);

	auto res = matrix_0.Multiply<3, 2>(matrix_1);

	double m_res[] = {22., 28., 49., 64., 76., 100.};
	const Matrix<double, 3, 2> res_matrix(m_res);
	EXPECT_EQ(res_matrix, res);
}

TEST(MarixTest, MultiplicationTest)
{
	Matrix4<float> view;
	Matrix4<float> model;
	view.Identity();
	model.Identity();

	view.CreateTranslation(VectorConstructor<float>::Construct(0, 0, -7));
	const auto res_0 = view.Multiply<4, 4>(model);
	const auto res_1 = view * model;
	EXPECT_EQ(res_0, res_1);
}

typedef Matrix4<float> Matrix4f;

/*
	Often Translate->Scale needed
	And not to multiply Matrix two times
	Create one matrix that gives this result
*/
TEST(MatrixTests, TranslateAndThanScale)
{
	Matrix4f translation_matrix(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		5, 5, 5, 1
		);

	Matrix4f scale_matrix(
		2.f, 0, 0, 0,
		0, 3.f, 0, 0,
		0, 0, 4.f, 0,
		0, 0, 0, 1
		);

	auto translate_scale = translation_matrix.Multiply<4, 4>(scale_matrix);
	std::cout << translate_scale << std::endl;
	Matrix4f matrix(
		1, 2, 3, 4,
		5, 6, 7, 8,
		9, 10, 11, 12,
		13, 14, 15, 16
		);

	Matrix4f current_matrix = matrix;
	current_matrix = current_matrix.Multiply<4, 4>(translation_matrix);	
	current_matrix = current_matrix.Multiply<4, 4>(scale_matrix);
	
	//current_matrix = current_matrix.Multiply<4, 4>(translation_matrix_1);
	std::cout << current_matrix << std::endl;

	Matrix4f current_matrix_0 = matrix;
	current_matrix_0 = current_matrix_0.Multiply<4, 4>(translate_scale);
	std::cout << current_matrix_0 << std::endl;

	EXPECT_EQ(current_matrix, current_matrix_0);
}

TEST(MatrixTests, TwoTranslations)
{
	Matrix4f translation_matrix(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		5, 5, 5, 1
		);

	Matrix4f translation_matrix_1(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		1, 1, 1, 1
		);

	Matrix4f uni_transform(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		6, 6, 6, 1
		);

	Matrix4f matrix(
		1, 2, 3, 4,
		5, 6, 7, 8,
		9, 10, 11, 12,
		13, 14, 15, 16
		);

	Matrix4f current_matrix = matrix;
	current_matrix = current_matrix.Multiply<4, 4>(translation_matrix);
	current_matrix = current_matrix.Multiply<4, 4>(translation_matrix_1);

	//current_matrix = current_matrix.Multiply<4, 4>(translation_matrix_1);
	std::cout << current_matrix << std::endl;

	Matrix4f current_matrix_0 = matrix;
	current_matrix_0 = current_matrix_0.Multiply<4, 4>(uni_transform);
	std::cout << current_matrix_0 << std::endl;

	EXPECT_EQ(current_matrix, current_matrix_0);
}

TEST(MatrixTests, MultiplyOnMatrixTest_ScaleOn2)
{
	double m_0[] = { 1., 2., 3., 4., 5., 6., 7., 8., 9. };
	double m_1[] = { 2., 0., 0., 0., 2., 0., 0., 0., 2.};

	Matrix<double, 3, 3> matrix_0(m_0);
	Matrix<double, 3, 3> scale_matrix(m_1);

	auto res = matrix_0.Multiply<3, 3>(scale_matrix);

	double m_res[] = { 2., 4., 6., 8., 10., 12., 14., 16., 18. };
	const Matrix<double, 3, 3> res_matrix(m_res);
	EXPECT_EQ(res_matrix, res);
}

TEST(MatrixTests, MultiplyOnValue)
{
	double m_1[] = { 1., 2., 3., 4. };
	const Matrix<double, 2, 2> matrix(m_1);

	auto result = matrix * 2.;

	EXPECT_EQ(2., result(0, 0));
	EXPECT_EQ(4., result(0, 1));
	EXPECT_EQ(6., result(1, 0));
	EXPECT_EQ(8., result(1, 1));
}

TEST(MatrixTests, TransposeTest)
{
	double m_1[] = { 1., 2., 3., 4. };
	const Matrix<double, 2, 2> matrix(m_1);

	auto result = matrix.GetTransposed();

	EXPECT_EQ(1., result(0, 0));
	EXPECT_EQ(3., result(0, 1));
	EXPECT_EQ(2., result(1, 0));
	EXPECT_EQ(4., result(1, 1));
}

TEST(MatrixTests, GenericInverseTest_ThrowIfMisNotSquare)
{
	Matrix<double, 2, 3> m;
	bool throwed = false;
	try
	{
		m.GetInverseGeneric();
	}
	catch (std::logic_error&)
	{
		throwed = true;
	}
	EXPECT_TRUE(throwed);
}

TEST(MatrixTests, GenericInverseTest)
{
	double m[] = {2., 3., 8., 6., 0., -3., -1., 3., 2.};
	Matrix<double, 3, 3> matrix(m);
	
	auto inverse = matrix.GetInverseGeneric();
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

TEST(MatrixTests, MakeTraslation4x4)
{
	typedef Vector<double, 3> Vector3;
	typedef Quaternion<double> QuaternionD;
	Matrix4<double> m;
	//m.MakeTransform(Vector3(), Vector3(), QuaternionD(1, 0, 0, 1));
}