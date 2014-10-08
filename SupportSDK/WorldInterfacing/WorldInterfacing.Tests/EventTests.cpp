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

//TEST(DeletableEventTests, GeneralTest_DefaultDeleter_OwnData_DestructorShouldBeCalled)
//	{
//	bool was_deleted = false;
//	TestClass* p_test = new TestClass(was_deleted);
//	if (true)
//		{
//		DeletableEvent<TestClass> e(1, p_test, true);
//		
//		EXPECT_EQ(1, e.m_code);
//		EXPECT_EQ(p_test, e.mp_data);
//		}
//	EXPECT_TRUE(was_deleted);
//	}
//
//TEST(DeletableEventTests, GeneralTest_DefaultDeleter_NotOwnData_DestructorShould_Not_BeCalled)
//	{
//	bool was_deleted = false;
//	TestClass* p_test = new TestClass(was_deleted);
//	if (true)
//		{
//		DeletableEvent<TestClass> e(1, p_test, false);
//		
//		EXPECT_EQ(1, e.m_code);
//		EXPECT_EQ(p_test, e.mp_data);
//		}
//	EXPECT_FALSE(was_deleted);
//	}
//
//TEST(DeletableEventTests, CopyConstructor_ShouldSkipDataOwnage)
//	{
//	bool was_deleted = false;
//	TestClass* p_test = new TestClass(was_deleted);
//
//	DeletableEvent<TestClass> e(1, p_test, true);
//	if (true)
//		{
//		DeletableEvent<TestClass> copied_EventBase(e);
//		EXPECT_EQ(e.mp_data, copied_EventBase.mp_data);
//		EXPECT_EQ(e.m_code, copied_EventBase.m_code);
//		}
//	EXPECT_FALSE(was_deleted);
//	}
//
//TEST(DeletableEventTests, MoveConstructor_ShouldTakeDataOwnage)
//	{
//	bool was_deleted = false;
//	TestClass* p_test = new TestClass(was_deleted);
//
//	DeletableEvent<TestClass> e(1, p_test, true);
//	if (true)
//		{
//		DeletableEvent<TestClass> copied_event(e);
//		EXPECT_EQ(e.mp_data, copied_event.mp_data);
//		EXPECT_EQ(e.m_code, copied_event.m_code);
//		}
//	EXPECT_TRUE(was_deleted);
//	}