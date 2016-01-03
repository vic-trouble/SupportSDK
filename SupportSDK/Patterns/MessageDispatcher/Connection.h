#ifndef __PATTERNS_MSGDISPATCHERCONNECTION_H__
#define __PATTERNS_MSGDISPATCHERCONNECTION_H__

#include "MessageDispatcher.h"

namespace SDK
{

	class Connection
	{
	private:
		MessageDispatcher& m_dispatcher;
		std::type_index m_type;
		std::string m_publisher;

		bool m_connected;

	public:
		template <typename HandlerType, typename EventType>
		Connection(MessageDispatcher& o_dispatcher, HandlerType& i_instance, void (HandlerType::*member_function)(const EventType&), const std::string& i_publisher)
			: m_type(typeid(EventType))
			, m_dispatcher(o_dispatcher)
			, m_publisher(i_publisher)
			, m_connected(true)
		{
			m_dispatcher.RegisterHandler<HandlerType, EventType>(i_instance, member_function, i_publisher);
		}
		~Connection()
		{
			disconnect();
		}

		bool connected() const
		{
			return m_connected;
		}
		void disconnect()
		{
			m_dispatcher.UnregisterHandler(m_type, m_publisher);
			m_connected = false;
		}
	};

} // SDK

#endif