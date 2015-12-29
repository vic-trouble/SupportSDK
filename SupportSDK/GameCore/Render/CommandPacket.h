#ifndef	__GAMECORE_COMMANDPACKET_H__
#define __GAMECORE_COMMANDPACKET_H__

namespace SDK
{
	
	namespace Render
	{
		
		namespace Commands
		{
			typedef void(*CommandExecutor)(const void*);
			typedef int CommandHandler;

			class CommandPacket
			{
			public:
				constexpr static int InvalidHandler = -1;
			public:
				void* mp_data;
				size_t m_data_size;
				CommandExecutor m_executor;
				CommandExecutor m_completion;
				CommandHandler m_next;
				bool m_executed;

				CommandPacket(const CommandPacket&) = delete;
				CommandPacket& operator = (const CommandPacket&) = delete;

				CommandPacket()
					: mp_data(nullptr)
					, m_data_size(0)
					, m_executed(false)
					, m_next(-1)
				{}

				CommandPacket(CommandPacket&& i_other)
					: mp_data(i_other.mp_data)
					, m_data_size(i_other.m_data_size)
					, m_executor(i_other.m_executor)
					, m_executed(i_other.m_executed)
					, m_completion(i_other.m_completion)
					, m_next(i_other.m_next)
				{
					i_other.mp_data = nullptr;
					i_other.m_data_size = 0;
				}

				~CommandPacket()
				{
					if (mp_data != nullptr)
						::operator delete(mp_data, m_data_size);
				}
			};

		} // Commands

	} // Render

} // SDLK

#endif
