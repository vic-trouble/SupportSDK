#pragma once

#include <vector>
#include <memory>

#include <time.h>

namespace CmdBucketTest
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

	////////////////////////////////////////////////////////
	// Global vars to access
	extern std::vector<int> g_core;
	extern std::vector<int> g_inh;

	////////////////////////////////////////////////////////
	// Test command for inheritance
	
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

	////////////////////////////////////////////////////////
	// Test results

	struct TestData
	{
		clock_t add_time;
		clock_t submit;
	};

	std::pair<TestData, TestData> Test();

} // CmdBucketTest

////////////////////////////////////////////////////////
// Test command for C-style
namespace SDK
{
	namespace Render
	{
		typedef void(*CommandExecutor)(const void*);
		struct TransformCore
		{

			void SetDefaultValues()
			{
				data = 1;
			}

			static void PushTransformation(const void* ip_data)
			{
				const TransformCore* cmd = reinterpret_cast<const TransformCore*>(ip_data);
				CmdBucketTest::g_core.push_back(cmd->data);
			}

			static void PopTransformation(const void* ip_data)
			{
				CmdBucketTest::g_core.pop_back();
			}
			static constexpr CommandExecutor EXECUTOR_FUNCTION = &PushTransformation;
			static constexpr CommandExecutor COMPLETION_FUNCTION = &PopTransformation;
			int data;
		};
	}
}
