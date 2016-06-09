#include "stdafx.h"

#include "CommandBucketTests.h"

#include <GameCore/Render/Commands.h>
#include <GameCore/Render/CommandBucket.h>

namespace PerformanceTests
{
	struct TestData
	{
		virtual ~TestData() {}
	};

	clock_t Runner(std::function<void()> func)
	{
		clock_t begin = clock();
		func();
		return clock() - begin;
	}

} // PerformanceTests

namespace Inherited
{
	class CommandPacket
	{
	public:
		constexpr static int InvalidHandler = -1;
	public:
		void Execute()
		{
			ExecuteImpl();
			if (mp_next != nullptr)
				mp_next->ExecuteImpl();
			Complete();
		}
		virtual void ExecuteImpl() = 0;
		virtual void Complete() {}
		CommandPacket* mp_next;
		bool m_executed;

		CommandPacket(const CommandPacket&) = delete;
		CommandPacket& operator = (const CommandPacket&) = delete;

		CommandPacket()
			: mp_next(nullptr)
		{}
		virtual ~CommandPacket() {}
	};

	////////////////////////////////////////////////////////

	template <typename KeyType = uint>
	class CommandBucket
	{
	public:
		using Key = KeyType;
	private:
		typedef std::unique_ptr<CommandPacket> CmdPacket;
		std::vector<CmdPacket> m_packets;

	public:
		CommandBucket()
		{	}

		template <typename ProcessorType>
		ProcessorType* Create()
		{
			CmdPacket p_cmd = std::make_unique<ProcessorType>();
			ProcessorType* p_raw = static_cast<ProcessorType*>(p_cmd.get());
			m_packets.push_back(std::move(p_cmd));
			return p_raw;
		}

		template <typename ProcessorType>
		ProcessorType* Append(CommandPacket* ip_command, size_t i_aux_mem_size = 0)
		{
			CmdPacket p_cmd = std::make_unique<ProcessorType>();
			ProcessorType* p_raw = p_cmd.get();
			m_packets.push_back(std::move(p_cmd));
			auto it = std::find_if(m_packets.begin(), m_packets.end(), [ip_command](const CommandPacket& cmd_packet)
			{
				return cmd_packet.get() == ip_command;
			});
			// TODO: notify user about error
			if (it != m_packets.end())
				it->m_next = it->get()
				;
			return p_raw;
		}

		void Sort() {}

		void Submit()
		{
			for (auto& packet : m_packets)
			{
				if (packet->m_executed)
					continue;
				packet->Execute();
			}
			m_packets.clear();
		}
	};
	
	static std::vector<int> g_inh;

	struct TransformCmd : public CommandPacket
	{
		TransformCmd()
		{
			data = 1;
		}
		virtual void ExecuteImpl() override
		{
			g_inh.push_back(data);
		}

		virtual void Complete() override
		{
			g_inh.pop_back();
		}
		int data;
	};

	struct CmdTestData : public PerformanceTests::TestData
	{
		CommandBucket<int> cmd_bucket;
		size_t add_time;
		size_t submit_time;
	};

	void TestAdd(CmdTestData& o_data, size_t i_count)
	{
		auto& cmd_bucket = o_data.cmd_bucket;
		for (size_t i = 0; i < i_count; ++i)
		{
			auto p_cmd = cmd_bucket.Create<TransformCmd>();
			p_cmd->data = 1;
		}
	}

	void TestSubmit(CmdTestData& o_data)
	{
		o_data.cmd_bucket.Submit();
	}

	CmdTestData Test(size_t i_count)
	{
		CmdTestData data;
		data.add_time = PerformanceTests::Runner([&data, i_count]() {TestAdd(data, i_count); });
		data.submit_time = PerformanceTests::Runner([&data]() {TestSubmit(data); });
		return data;
	}
} // Inherited

namespace CStyle
{
	static std::vector<int> g_core;
	typedef void(*CommandExecutor)(const void*);
	struct TransformCmd
	{
		void SetDefaultValues()
		{
			data = 1;
		}

		static void PushTransformation(const void* ip_data)
		{
			const TransformCmd* cmd = reinterpret_cast<const TransformCmd*>(ip_data);
			g_core.push_back(cmd->data);
		}

		static void PopTransformation(const void* ip_data)
		{
			g_core.pop_back();
		}
		static constexpr CommandExecutor EXECUTOR_FUNCTION = &PushTransformation;
		static constexpr CommandExecutor COMPLETION_FUNCTION = &PopTransformation;
		int data;
	};

	using namespace SDK::Render::Commands;
	struct CmdTestData : public PerformanceTests::TestData
	{
		CommandBucket<int> cmd_bucket;
		size_t add_time;
		size_t submit_time;
	};

	void TestAdd(CmdTestData& o_data, size_t i_count)
	{
		auto& cmd_bucket = o_data.cmd_bucket;
		for (size_t i = 0; i < i_count; ++i)
		{
			auto p_cmd = cmd_bucket.Create<TransformCmd>();
			p_cmd->data = 1;
		}
	}

	void TestSubmit(CmdTestData& o_data)
	{
		o_data.cmd_bucket.Submit();
	}

	CmdTestData Test(size_t i_count)
	{
		CmdTestData data;
		data.add_time = PerformanceTests::Runner([&data, i_count]() {TestAdd(data, i_count); });
		data.submit_time = PerformanceTests::Runner([&data]() {TestSubmit(data); });
		return data;
	}
} // CStyle

namespace CmdBucketTest
{	
	void Test()
	{
		static constexpr size_t COUNT = 1000000;
		auto c_style_result = CStyle::Test(COUNT);
		auto inh_result = Inherited::Test(COUNT);

		std::cout << "-----------------------------------" << std::endl;
		std::cout << "\tCStyle add: " << c_style_result.add_time << std::endl
			<< "\tInh add: " << inh_result.add_time << std::endl
			<< "\tCStyle submit: " << c_style_result.submit_time << std::endl
			<< "\tInh submit: " << inh_result.submit_time << std::endl;
	}
}