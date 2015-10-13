#include "stdafx.h"

#include <Quaternion.h>

using namespace SDK::Math;

typedef Quaternion<float> QuaternionF;

namespace
{
	template <typename CoordinateType>
	std::ostream& operator << (std::ostream& o_stream, const Quaternion<CoordinateType>& i_q)
	{
		o_stream << i_q[0] << " " << i_q[1] << " " << i_q[2] << " " << i_q[3] << std::endl;
		return o_stream;
	}
} // namespace

TEST(QuaternionTests, EmptyConstructor)
{
	QuaternionF q;

	EXPECT_EQ(0, q[0]);
	EXPECT_EQ(0, q[1]);
	EXPECT_EQ(0, q[2]);
	EXPECT_EQ(0, q[3]);
}

TEST(QuaternionTests, BasicConstructor)
{
	QuaternionF q(1, 2, 3, 4);

	EXPECT_EQ(1, q[0]);
	EXPECT_EQ(2, q[1]);
	EXPECT_EQ(3, q[2]);
	EXPECT_EQ(4, q[3]);
}

TEST(QuaternionTests, CopyConstructor)
{
	QuaternionF q(1, 2, 3, 4);

	QuaternionF q1(q);

	EXPECT_EQ(1, q1[0]);
	EXPECT_EQ(2, q1[1]);
	EXPECT_EQ(3, q1[2]);
	EXPECT_EQ(4, q1[3]);
}

TEST(QuaternionTests, CopyOperator)
{
	QuaternionF q(1, 2, 3, 4);

	QuaternionF q1 = q;

	EXPECT_EQ(1, q1[0]);
	EXPECT_EQ(2, q1[1]);
	EXPECT_EQ(3, q1[2]);
	EXPECT_EQ(4, q1[3]);
}

TEST(QuaternionTests, MultiplyOperator)
{
	QuaternionF q1(-2.1f, 3.f, 4.f, 3.f);
	QuaternionF q2(4.f, 3.9f, -1.f, -3.f);

	auto res = q1*q2;

	EXPECT_FLOAT_EQ(-7.1f, res[0]);
	EXPECT_FLOAT_EQ(-5.19f, res[1]);
	EXPECT_FLOAT_EQ(38.8f, res[2]);
	EXPECT_FLOAT_EQ(-0.30000114f, res[3]);
}

TEST(QuaternionTests, LengthTest)
{
	QuaternionF q(4.f, 4.f, 4.f, 4.f);

	EXPECT_EQ(64, q.LengthSq());
	EXPECT_EQ(8, q.Length());
}

TEST(QuaternionTests, NormalizeTest)
{
	QuaternionF q(4.f, 3.9f, -1.f, -3.f);

	q.Normalize();
	EXPECT_FLOAT_EQ(0.62310135f, q[0]);
	EXPECT_FLOAT_EQ(0.60752386f, q[1]);
	EXPECT_FLOAT_EQ(-0.15577534f, q[2]);
	EXPECT_FLOAT_EQ(-0.46732602f, q[3]);
}

TEST(QuaternionTests, ConjugateTest)
{
	QuaternionF q(4.f, 3.9f, -1.f, -3.f);

	QuaternionF q_trans(4.f, 3.9f, -1.f, -3.f);

	q_trans.Conjugate();
	EXPECT_FLOAT_EQ(-q[1], q_trans[1]);
	EXPECT_FLOAT_EQ(-q[2], q_trans[2]);
	EXPECT_FLOAT_EQ(-q[3], q_trans[3]);
}

TEST(QuaternionTests, RotateVectorStandard)
{
	QuaternionF q(4.f, 3.9f, -1.f, -3.f);
	Vector<float, 3> vec = VectorConstructor<float>::Construct(1.5f, 3.7f, -25.f);

	const auto res = q.ApplyTo(vec);

	EXPECT_FLOAT_EQ(816.440002f, res[0]);
	EXPECT_FLOAT_EQ(406.846008f, res[1]);
	EXPECT_FLOAT_EQ(900.040038f, res[2]);
}

TEST(QuaternionTests, DotProductTest)
{
	QuaternionF first(1, 0, 0, 1);
	QuaternionF second(1, 0, 1, 0);

	EXPECT_EQ(1, first.DotProduct(second));
	EXPECT_EQ(1, second.DotProduct(first));
	EXPECT_EQ(2, second.DotProduct(second));
}

TEST(QuaternionTests, SlerpTestNegativeCosine_ShortestPathFalse)
{
	QuaternionF q1(-2.1f, 3.f, 4.f, 3.f);
	QuaternionF q2(4.f, 3.9f, -1.f, -3.f);

	auto result = QuaternionF::Slerp(0.2f, q1, q2, false);

	EXPECT_FLOAT_EQ(-0.182988957f, result[0]);
	EXPECT_FLOAT_EQ(0.661255598f, result[1]);
	EXPECT_FLOAT_EQ(0.623826027f, result[2]);
	EXPECT_FLOAT_EQ(0.374295622f, result[3]);
}

TEST(QuaternionTests, SlerpTestNegativeCosine_ShortestPathTrue)
{
	QuaternionF q1(-2.1f, 3.f, 4.f, 3.f);
	QuaternionF q2(4.f, 3.9f, -1.f, -3.f);

	auto result = QuaternionF::Slerp(0.2f, q1, q2, true);

	EXPECT_FLOAT_EQ(-0.457888901f, result[0]);
	EXPECT_FLOAT_EQ(0.299104869f, result[1]);
	EXPECT_FLOAT_EQ(0.627750933f, result[2]);
	EXPECT_FLOAT_EQ(0.553897858f, result[3]);
}

TEST(QuaternionTests, SlerpTestPositiveCosine)
{
	QuaternionF q1(2.1f, 3.f, 4.f, 3.f);
	QuaternionF q2(4.f, 3.9f, -1.f, -3.f);

	auto res_short_path_false = QuaternionF::Slerp(0.2f, q2, q1, false);
	auto res_short_path_true = QuaternionF::Slerp(0.2f, q2, q1, true);

	EXPECT_FLOAT_EQ(0.658914089f, res_short_path_false[0]);
	EXPECT_FLOAT_EQ(0.677116156f, res_short_path_false[1]);
	EXPECT_FLOAT_EQ(0.f, res_short_path_false[2]);
	EXPECT_FLOAT_EQ(-0.327636838f, res_short_path_false[3]);

	EXPECT_EQ(res_short_path_false, res_short_path_true);
}

TEST(QuaternionTests, NlerpTestNegativeCosine)
{
	QuaternionF q1(-2.1f, 3.f, 4.f, 3.f);
	QuaternionF q2(4.f, 3.9f, -1.f, -3.f);

	auto res_short_path_false = QuaternionF::Nlerp(0.55f, q2, q1, false);

	EXPECT_FLOAT_EQ(0.165643185f, res_short_path_false[0]);
	EXPECT_FLOAT_EQ(0.874441922f, res_short_path_false[1]);
	EXPECT_FLOAT_EQ(0.449419498f, res_short_path_false[2]);
	EXPECT_FLOAT_EQ(0.0770433918f, res_short_path_false[3]);
}

TEST(QuaternionTests, NlerpTestPositiveCosine)
{
	QuaternionF q1(2.1f, 3.f, 4.f, 3.f);
	QuaternionF q2(4.f, 3.9f, -1.f, -3.f);

	auto res_short_path_false = QuaternionF::Nlerp(0.55f, q2, q1, false);
	auto res_short_path_true = QuaternionF::Nlerp(0.55f, q2, q1, true);

	EXPECT_FLOAT_EQ(0.609848320f, res_short_path_false[0]);
	EXPECT_FLOAT_EQ(0.702718675f, res_short_path_false[1]);
	EXPECT_FLOAT_EQ(0.361162305f, res_short_path_false[2]);
	EXPECT_FLOAT_EQ(0.061913576f, res_short_path_false[3]);

	EXPECT_EQ(res_short_path_false, res_short_path_true);
}

TEST(QuaternionTests, QuaternionTypes)
{
	Quaternion<float> float_quat;
	Quaternion<double> double_quat;

	//Quaternion<int> int_quat; // compileation error
}