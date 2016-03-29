#include "stdafx.h"

#include <VectorUtilities.h>
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

// use of partially specialized template
TEST(VectorUtilitiesTests, Projection_3D)
	{
	Vector<double, 3> vec_0;
	vec_0[0] = 2; vec_0[1] = 2; vec_0[2] = 2;
	Vector<double, 3> vec_1;
	vec_1[0] = 3; vec_1[1] = 3; vec_1[2] = 3;

	Vector<double, 3> projection;
	projection[0] = 2; projection[1] = 2; projection[2] = 2;

	auto result = VectorUtilities<double, 3>::Projection(vec_0, vec_1);
	EXPECT_EQ(projection, result);
	}

// use of partially specialized template
TEST(VectorUtilitiesTests, Perpendicular_3D)
	{
	Vector<double, 3> vec_0;
	vec_0[0] = 2; vec_0[1] = 2; vec_0[2] = 2;
	Vector<double, 3> vec_1;
	vec_1[0] = 3; vec_1[1] = 3; vec_1[2] = 3;

	Vector<double, 3> projection;

	auto result = VectorUtilities<double, 3>::PerpendicularComponent(vec_0, vec_1);
	EXPECT_EQ(projection, result);
	}

TEST(VectorUtilitiesTests, OneVectorInContainer_IsIndependentShouldReturn_True)
	{
	Vector<int, 3> vec;
	vec[0] = 1; vec[1] = 0; vec[2] = 1;
	std::vector<Vector<int,3>> vectors;
	vectors.push_back(vec);
	bool is_independent = VectorUtilities<int, 3>::IsVectorsLinearyIndependent(vectors);
	EXPECT_TRUE(is_independent);
	}

TEST(VectorUtilitiesTests, TwoIdenticalVectors_IsIndependentShouldReturn_False)
	{
	Vector<int, 3> vec;
	vec[0] = 1; vec[1] = 0; vec[2] = 1;
	std::vector<Vector<int,3>> vectors;
	vectors.push_back(vec);
	vectors.push_back(vec);
	bool is_independent = VectorUtilities<int, 3>::IsVectorsLinearyIndependent(vectors);
	EXPECT_FALSE(is_independent);
	}

TEST(VectorUtilitiesTests, BasisVectors_IsIndependentShouldReturn_True)
	{
	Vector<int, 3> i, j, k;
	i[0] = 1; i[1] = 0; i[2] = 0;
	j[0] = 0; j[1] = 1; j[2] = 0;
	k[0] = 0; k[1] = 0; k[2] = 1;

	std::vector<Vector<int,3>> vectors;
	vectors.push_back(i);
	vectors.push_back(j);
	vectors.push_back(k);

	bool is_independent = VectorUtilities<int, 3>::IsVectorsLinearyIndependent(vectors);
	EXPECT_TRUE(is_independent);
	}

TEST(VectorUtilitietsTests, VectorSpaces_ShouldGiveOrthonormalVectors)
	{
	Vector<double, 3> a = VectorConstructor<double>::Construct(sqrt(2)/2, sqrt(2)/2, 0);

	Vector<double, 3> b = VectorConstructor<double>::Construct(-1, 1, -1);

	Vector<double, 3> c = VectorConstructor<double>::Construct(0, -2, -2);

	std::vector<Vector<double, 3>> vectors;
	vectors.push_back(a);
	vectors.push_back(b);
	vectors.push_back(c);

	Vector<double, 3> d = VectorConstructor<double>::Construct(1, -1, -2);

	auto orthonormal_vectors = VectorUtilities<double, 3>::ComputeOrthonormalVectors(vectors);
	EXPECT_EQ(a, orthonormal_vectors[0]);
	EXPECT_EQ(b, orthonormal_vectors[1]);
	EXPECT_EQ(d, orthonormal_vectors[2]);
	}