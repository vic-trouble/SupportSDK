#include "stdafx.h"

#include <Vector.h>
#include <VectorConstructor.h>

using namespace SDK::Math;

namespace
	{
	template <typename CoordinateType, size_t Dimension>
	std::ostream& operator << (std::ostream& o_stream, const Vector<CoordinateType, Dimension>& i_vector)
		{
		for (size_t i = 0; i < Dimension; ++i)
			o_stream << i_vector[i] << " ";
		return o_stream;
		}
	} // namespace

TEST(VectorTests, ConstructVector_VerticesShouldBeZero)
	{
	Vector<double, 3> vec;
	for (size_t i = 0; i < Vector<double, 3>::VertexNumber; ++i)
		EXPECT_EQ(0, vec[i]);
	EXPECT_TRUE(vec.IsZero());
	}

TEST(VectorTests, AssignOperator_ShouldAssignCorrectValues)
	{
	Vector<int, 3> vec;
	
	vec[1] = 5;
	EXPECT_EQ(5, vec[1]);
	EXPECT_FALSE(vec.IsZero());
	}

TEST(VectorTests, MultiplyBy5_VerticesShouldBe20)
	{
	Vector<int, 3> vec;
	for (size_t i = 0; i < Vector<double, 3>::VertexNumber; ++i)
		vec[i] = 4;
	vec *= 5;
	for (size_t i = 0; i < Vector<double, 3>::VertexNumber; ++i)
		EXPECT_EQ(20, vec[i]);
	}

TEST(VectorTests, DevideBy5_VerticesShouldBe4)
	{
	Vector<int, 3> vec;
	for (size_t i = 0; i < Vector<double, 3>::VertexNumber; ++i)
		vec[i] = 20;
	vec /= 5;
	for (size_t i = 0; i < Vector<double, 3>::VertexNumber; ++i)
		EXPECT_EQ(4, vec[i]);
	}

TEST(VectorTests, AddVector_VerticesShouldBe20)
	{
	Vector<int, 3> vec_0;
	Vector<int, 3> vec_1;
	for (size_t i = 0; i < Vector<double, 3>::VertexNumber; ++i)
		{
		vec_0[i] = 10;
		vec_1[i] = 10;
		}
	vec_0 += vec_1;
	for (size_t i = 0; i < Vector<double, 3>::VertexNumber; ++i)
		EXPECT_EQ(20, vec_0[i]);
	}

TEST(VectorTests, MinusVector_VerticesShouldBeMinus10)
	{
	Vector<int, 3> vec_0;
	Vector<int, 3> vec_1;
	for (size_t i = 0; i < Vector<double, 3>::VertexNumber; ++i)
		{
		vec_0[i] = 10;
		vec_1[i] = 20;
		}
	vec_0 -= vec_1;
	for (size_t i = 0; i < Vector<double, 3>::VertexNumber; ++i)
		EXPECT_EQ(-10, vec_0[i]);
	}

TEST(VectorTests, LengthSqShouldBe300_LengthShouldBe_SqrtFromLength)
	{
	Vector<int, 3> vec;
	for (size_t i = 0; i < Vector<int, 3>::VertexNumber; ++i)
		vec[i] = 10;
	EXPECT_EQ(300, vec.LengthSq());
	EXPECT_EQ(sqrt(300), vec.Length());// near 17.3205
	}

TEST(VectorTests, Normalize_LengthShouldBe1)
	{
	Vector<double, 3> vec;
	for (size_t i = 0; i <  Vector<int, 3>::VertexNumber; ++i)
		vec[i] = 5.;

	vec.Normalize();
	EXPECT_NEAR(0.577367, vec[0], 0.0001);
	EXPECT_NEAR(0.577367, vec[1], 0.0001);
	EXPECT_NEAR(0.577367, vec[2], 0.0001);
	EXPECT_EQ(1, vec.LengthSq());
	}

TEST(VectorTests, DotProductShouldBe_27)
	{
	Vector<int, 3> vec_0;
	Vector<int, 3> vec_1;
	for (size_t i = 0; i < 3; ++i)
		{
		vec_0[i] = 3;
		vec_1[i] = 3;
		}

	EXPECT_EQ(27, vec_0.DotProduct(vec_1));
	}

TEST(VectorTests, DotProductShouldBe_ZeroForPerpendicular)
	{
	Vector<int, 3> vec_0;
	Vector<int, 3> vec_1;
	vec_0[0] = -12;
	vec_0[1] = 16;
	vec_1[0] = 12;
	vec_1[1] = 9;

	EXPECT_EQ(0, vec_0.DotProduct(vec_1));
	}

TEST(VectorTests, CrossProduct_TwoDimension)
	{
	Vector<int, 2> vec_0 = VectorConstructor<int>::Construct(1, 2);
	Vector<int, 2> vec_1 = VectorConstructor<int>::Construct(3, 4);

	auto cross_product = vec_0.CrossProduct(vec_1);
	EXPECT_EQ(-2, cross_product[0]);
	EXPECT_EQ(0, cross_product[1]);
	}

TEST(VectorTests, CrossProduct_ThreeDimension)
	{
	Vector<int, 3> vec_0;
	Vector<int, 3> vec_1;
	vec_0[0] = 2;	vec_0[1] = 3;	vec_0[2] = 4;
	vec_1[0] = 5;	vec_1[1] = 6;	vec_1[2] = 7;

	auto cross_product = vec_0.CrossProduct(vec_1);
	EXPECT_EQ(-3, cross_product[0]);
	EXPECT_EQ(6, cross_product[1]);
	EXPECT_EQ(-3, cross_product[2]);
	}

TEST(VectorTests, CrossProduct_ThreeDimension_ZeroForOrthogonal)
	{
	Vector<int, 3> vec_0;
	Vector<int, 3> vec_1;
	vec_0[0] = 1;	vec_0[1] = 0;	vec_0[2] = 0;
	vec_1[0] = 0;	vec_1[1] = 1;	vec_1[2] = 0;

	auto cross_product = vec_0.CrossProduct(vec_1);
	Vector<int, 3> vec_2;
	vec_2[0] = 0;	vec_2[1] = 0;	vec_2[2] = 1;
	EXPECT_EQ(vec_2, cross_product);
	}

TEST(VectorTests, CrossProductPonQThanDotProductOnQ_ShouldResultZero)
	{
	Vector<int, 3> p;
	Vector<int, 3> q;
	p[0] = 2;	p[1] = 3;	p[2] = 4;
	q[0] = 5;	q[1] = 6;	q[2] = 7;

	auto cross_product = p.CrossProduct(q);
	ASSERT_EQ(0, cross_product.DotProduct(q));
	}

TEST(VectorTests, VectorAreEqual)
	{
	Vector<double, 3> vec_0;
	Vector<double, 3> vec_1;
	for (size_t i = 0; i < 3; ++i)
		{
		vec_0[i] = i;
		vec_1[i] = i;
		}
	EXPECT_TRUE(vec_0.operator==(vec_1));
	}

TEST(VectorTests, VectorAreNotEqual)
	{
	Vector<double, 3> vec_0;
	Vector<double, 3> vec_1;
	for (size_t i = 0; i < 3; ++i)
		{
		vec_0[i] = i;
		vec_1[i] = i+1;
		}
	EXPECT_FALSE(vec_0.operator==(vec_1));
	}