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

	void TestCalls()
	{
		MessageDispatcher dispatcher;
		TestHandler handler;

		// all publishers
		dispatcher.RegisterHandler(handler, &TestHandler::Handle, "handler");
		dispatcher.RegisterHandler(handler, &TestHandler::Handle, "handler");
		// specify publishers
		dispatcher.RegisterHandler(handler, &TestHandler::Handle, "handler", "publisher");
		dispatcher.RegisterHandler(handler, &TestHandler::Handle, "handler", "publisher");
		// different publishers, same id
		dispatcher.RegisterHandler(handler, &TestHandler::Handle, "handler_0", "publisher");
		dispatcher.RegisterHandler(handler, &TestHandler::Handle, "handler_0", "publisher1");
		// different ids, same publishers
		dispatcher.RegisterHandler(handler, &TestHandler::Handle, "handler_id", "publisher");
		dispatcher.RegisterHandler(handler, &TestHandler::Handle, "handler_id1", "publisher");

		dispatcher.UnregisterHandler<TestEvent>("handler");
		dispatcher.UnregisterHandler<TestEvent>("handler");

		dispatcher.UnregisterHandler<TestEvent>("handler", "publisher");
		
		dispatcher.UnregisterHandler<TestEvent>("handler_0", "publisher");
		dispatcher.UnregisterHandler<TestEvent>("handler_0", "publisher1");

		dispatcher.UnregisterHandler<TestEvent>("handler_id", "publisher");
		dispatcher.UnregisterHandler<TestEvent>("handler_id1", "publisher");

		dispatcher.RegisterHandler<TestEvent>([](const TestEvent& i_evt)
		{
			std::cout << "asdasd" << std::endl;
		}, "test", "pub");
		dispatcher.HandleMessage(TestEvent(), "pub");
		dispatcher.UnregisterHandler<TestEvent>("test", "pub");
	}

	void Test()
	{
		std::cout << "==========================================================" << std::endl;
		std::cout << "\t\tMessage dispatcher tests" << std::endl;
		TestCalls();
		MessageDispatcher dispatcher;
		TestHandler handler;
		// dummy publishers
		for (size_t i = 0; i < 100; ++i)
		{
			dispatcher.RegisterHandler(handler, &TestHandler::Handle, "handler", "test_"+ SDK::Utilities::lexical_cast(i));
			dispatcher.RegisterHandler(handler, &TestHandler::Handle1, "handler", "test1_" + SDK::Utilities::lexical_cast(i));
		}
		dispatcher.RegisterHandler(handler, &TestHandler::Handle, "handler", "test");
		dispatcher.RegisterHandler(handler, &TestHandler::Handle1, "handler", "test1");

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