#ifndef	__GAMECORE_COMMANDBUCKET_H__
#define __GAMECORE_COMMANDBUCKET_H__

#include "CommandPacket.h"

namespace SDK
{
	namespace Render
	{		
		namespace Commands
		{

			template <typename KeyType = uint>
			class CommandBucket
			{
			public:
				using Key = KeyType;
			private:
				std::vector<CommandPacket> m_packets;

			private:
				void Submit(CommandPacket& i_packet)
				{
					i_packet.m_executor(i_packet.mp_data);
					if (i_packet.mp_next != nullptr)
						Submit(*i_packet.mp_next);
					i_packet.m_completion(i_packet.mp_data);
					i_packet.m_executed = true;
				}

				template <typename ProcessorType>
				CommandPacket CreateImpl(size_t i_aux_mem_size)
				{
					void* p_data = ::operator new(sizeof(ProcessorType) + i_aux_mem_size);
					CommandPacket command;
					command.mp_data = p_data;
					command.m_data_size = i_aux_mem_size;
					command.m_executor = ProcessorType::EXECUTOR_FUNCTION;
					command.m_completion = ProcessorType::COMPLETION_FUNCTION;
					ProcessorType* p_processor = reinterpret_cast<ProcessorType*>(p_data);
					// POD data cannot contain constructor - so call this method to init
					p_processor->SetDefaultValues();
					return std::move(command);
				}

			public:
				template <typename ProcessorType>
				ProcessorType* Create(size_t i_aux_mem_size = 0)
				{
					CommandPacket command = CreateImpl<ProcessorType>(i_aux_mem_size);
					void* p_data = command.mp_data;
					m_packets.push_back(std::move(command));					
					return reinterpret_cast<ProcessorType*>(p_data);
				}

				template <typename ProcessorType>
				ProcessorType* Append(void* ip_command, size_t i_aux_mem_size = 0)
				{
					CommandPacket command = CreateImpl<ProcessorType>(i_aux_mem_size);
					void* p_data = command.mp_data;
					m_packets.push_back(std::move(command));
					auto it = std::find_if(m_packets.begin(), m_packets.end(), [ip_command](const CommandPacket& cmd_packet)
					{
						return cmd_packet.mp_data == ip_command;
					});
					// TODO: notify user about error
					if (it != m_packets.end())					
						it->mp_next = &m_packets.back();
					return reinterpret_cast<ProcessorType*>(p_data);
				}

				template <typename PtrType, typename T>
				PtrType* GetAuxilaryPtr(T* ip_command)
				{
					return reinterpret_cast<U>(reinterpret_cast<char*>(ip_command) + sizeof(T));
				}

				void Sort() {}

				void Submit()
				{
					for (auto& packet : m_packets)
					{
						if (packet.m_executed)
							continue;
						Submit(packet);
					}
					m_packets.clear();
				}
			};

		} // Commands
	} // Render
} // SDK

#endif
