#include "stdafx.h"

#include <Event.h>

using namespace SDK;

struct TestClass
		{
		bool& m_was_deleted;
		TestClass (bool& o_was_deleted)
			: m_was_deleted (o_was_deleted)
			{
			o_was_deleted = false;
			}

		~TestClass()
			{
			m_was_deleted = true;
			}
		};

TEST(EventBaseTests, GeneralTest_DestructorShouldNotBeCalled)
	{
	bool was_deleted = false;
	std::unique_ptr<TestClass> p_test (new TestClass(was_deleted));
	if (true)
		{
		EventBase e(1, p_test.get());
		EXPECT_EQ(1, e.m_code);
		EXPECT_EQ(p_test.get(), e.mp_data);
		}
	EXPECT_FALSE(was_deleted);
	}