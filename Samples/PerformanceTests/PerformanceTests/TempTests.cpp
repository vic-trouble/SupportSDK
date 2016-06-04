#include "stdafx.h"

#include "TempTests.h"

#include <GameCore/GenericHandlesDynamicArray.h>

namespace IfVsHandlers
{

	struct Message
	{
		std::string publisher;
		std::string data;

		Message(const std::string& publ, const std::string& d)
			: data(d)
			, publisher(publ)
		{}

		bool is(const std::string& d) const
		{
			return d == data;
		}
	};

	void X(int& o_d)
	{
		static int count = 0;
		o_d = count++;
	}

	void X1(int& o_d)
	{
		static int count = 0;
		o_d = count++;
	}

	void X2(int& o_d)
	{
		static int count = 0;
		o_d = count++;
	}

	bool AcceptMessage_IfStyle(const Message& msg)
	{
		int data = 0;
		if (msg.is("first"))
		{
			X(data);
			return true;
		}
		else if (msg.is("second"))
		{
			X1(data);
			return true;
		}
		else if (msg.is("third"))
		{
			X2(data);
			return true;
		}

		return false;
	}

	struct Handler
	{
		int data = 0;
		virtual bool Handle(const Message& msg) = 0;
	};

	struct FirstHandler : Handler
	{
		virtual bool Handle(const Message& msg) override
		{
			if (msg.is("first"))
			{
				X(data);
				return true;
			}
			return false;
		}
	};

	struct SecondHandler : Handler
	{
		virtual bool Handle(const Message& msg) override
		{
			if (msg.is("second"))
			{
				X(data);
				return true;
			}
			return false;
		}
	};

	struct ThirdHandler : Handler
	{
		virtual bool Handle(const Message& msg) override
		{
			if (msg.is("third"))
			{
				X(data);
				return true;
			}
			return false;
		}
	};

	struct Disp
	{
		std::vector<std::unique_ptr<Handler>> m_handlers;
		Disp()
		{
			m_handlers.push_back(std::make_unique<FirstHandler>());
			m_handlers.push_back(std::make_unique<SecondHandler>());
			m_handlers.push_back(std::make_unique<ThirdHandler>());
		}
		bool HandleMessage(const Message& msg)
		{
			for (auto& h : m_handlers)
			{
				if (h->Handle(msg))
					return true;
			}
			return false;
		}
	};

	void AcceptMessage_Handlers(const Message& msg, Disp& d)
	{
		d.HandleMessage(msg);
	}

	clock_t Runner(std::function<void()> func, size_t count)
	{
		clock_t begin = clock();
		for (size_t i = 0; i < count; ++i)
			func();
		return clock() - begin;
	}
	void Test()
	{
		constexpr size_t COUNT = 100000000;
		Message msg("publisher", "first");
		Disp d;
		clock_t if_style_t = Runner([&msg]() {AcceptMessage_IfStyle(msg); }, COUNT);
		clock_t handlers_t = Runner([&msg, &d]() {AcceptMessage_Handlers(msg, d); }, COUNT);
		std::cout << "if_style: " << if_style_t << std::endl
			 << "handlers: " << handlers_t << std::endl;
	}

} // IfVsHandlers

namespace TempTests
{

	struct VertexHandleTag {};
	typedef SDK::GenericHandle<int, 12, 20, VertexHandleTag> VertexBufferHandle;

	typedef SDK::GenericHandleDynamicArray<VertexBufferHandle, int> VertexBuffers;
	typedef SDK::GenericHandleDynamicArray<VertexBufferHandle, std::unique_ptr<int>> VertexBuffersPtrs;

	void TestBuffers()
	{
		VertexBuffers bufs;
		auto handle_i = bufs.CreateNew(5);
		int* ptr_i = bufs.Access(handle_i);
		bufs.Destroy(handle_i);

		VertexBuffersPtrs ptr_bufs;
		auto handle_ptr = ptr_bufs.CreateNew(9);
		int* ptr_i_ptr = ptr_bufs.Access(handle_ptr);
		ptr_bufs.Destroy(handle_ptr);
	}

	void Test()
	{
		TestBuffers();
		IfVsHandlers::Test();
	}

} // TempTests