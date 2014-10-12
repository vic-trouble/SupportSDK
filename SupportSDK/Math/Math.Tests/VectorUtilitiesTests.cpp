#include "stdafx.h"

#include <VectorUtilities.h>

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