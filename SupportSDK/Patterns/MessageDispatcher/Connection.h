#ifndef __PATTERNS_MSGDISPATCHERCONNECTION_H__
#define __PATTERNS_MSGDISPATCHERCONNECTION_H__

#include "MessageDispatcher.h"

namespace SDK
{

	class Connection
	{
	private:
		MessageDispatcher* mp_dispatcher;
		std::type_index m_type;
		std::string m_publisher;

	public:
		Connection(const Connection&) = delete;
		Connection& operator = (const Connection&) = delete;

		Connection()
			: m_type(typeid(*this))
			, mp_dispatcher(nullptr)
		{}

		Connection(Connection&& right)
			: mp_dispatcher(right.mp_dispatcher)
			, m_publisher(right.m_publisher)
			, m_type(right.m_type)
		{
			right.mp_dispatcher = nullptr;
		}

		Connection& operator = (Connection&& right)
		{
			mp_dispatcher = right.mp_dispatcher;
			m_publisher = right.m_publisher;
			m_type = right.m_type;
			right.mp_dispatcher = nullptr;
			return *this;
		}

		template <typename HandlerType, typename EventType>
		Connection(MessageDispatcher& o_dispatcher, HandlerType& i_instance, void (HandlerType::*member_function)(const EventType&), const std::string& i_publisher)
			: m_type(typeid(EventType))
			, mp_dispatcher(&o_dispatcher)
			, m_publisher(i_publisher)
		{
			mp_dispatcher->RegisterHandler<HandlerType, EventType>(i_instance, member_function, i_publisher);
		}
		~Connection()
		{
			disconnect();
		}

		bool connected() const
		{
			return mp_dispatcher != nullptr;
		}
		void disconnect()
		{
			if (mp_dispatcher == nullptr)
				return;

			mp_dispatcher->UnregisterHandler(m_type, m_publisher);
			mp_dispatcher = nullptr;
		}
	};

} // SDK

#endif