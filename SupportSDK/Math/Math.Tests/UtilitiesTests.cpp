#include "stdafx.h"

#include <Utilities.h>
#include <VectorConstructor.h>

using namespace SDK::Math;

typedef Vector<float, 2> Vector2;
typedef Line<float, 2> Line2;
typedef VectorConstructor<float> VecConstructorF;

TEST(UtilitiesTests, DistanceFromPointToLine)
{
	Line2 line(VecConstructorF::Construct(0.f, 0.f), VecConstructorF::Construct(1.f, 0.f));

	Vector2 v = VecConstructorF::Construct(0.5f, 1.5f);

	auto dist = Distance(v, line);
	EXPECT_EQ(1.5f, dist);
}

