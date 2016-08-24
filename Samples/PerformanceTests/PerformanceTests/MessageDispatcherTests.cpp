#include "stdafx.h"

#include "MessageDispatcherTests.h"
#include <Patterns/MessageDispatcher/MessageDispatcher.h>
#include <Utilities/lexical_cast.h>

namespace MessageDispatcherTests
{

	struct TestEvent : public Event
	{};

	struct TestEvent1 : public Event
	{};

	struct TestHandler
	{
		void Handle(const TestEvent& e)
		{		}
		void Handle1(const TestEvent1& e)
		{		}
	};

	void Test()
	{
		std::cout << "==========================================================" << std::endl;
		std::cout << "\t\tMessage dispatcher tests" << std::endl;
		MessageDispatcher dispatcher;
		TestHandler handler;
		// dummy publishers
		for (size_t i = 0; i < 100; ++i)
		{
			dispatcher.RegisterHandler(handler, &TestHandler::Handle, "test_"+ SDK::Utilities::lexical_cast(i));
			dispatcher.RegisterHandler(handler, &TestHandler::Handle1, "test1_" + SDK::Utilities::lexical_cast(i));
		}
		dispatcher.RegisterHandler(handler, &TestHandler::Handle, "test");
		dispatcher.RegisterHandler(handler, &TestHandler::Handle1, "test1");

		constexpr static size_t COUNT = 1000000;
		TestEvent e;
		TestEvent1 e1;
		clock_t beg = clock();
		for (size_t i = 0; i < COUNT; ++i)
		{
			dispatcher.HandleMessage(e, "test");
		}
		clock_t disp_time = clock() - beg;
		std::cout << "Time: " << disp_time << std::endl;
		std::cout << "==========================================================" << std::endl;
	}

} // MessageDispatcherTests