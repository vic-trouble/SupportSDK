#pragma once

#include "../PatternsAPI.h"

#include "MessageHandlerBase.h"
#include "MemberFunctionHandler.h"

#include <unordered_map>

#include <Utilities/HashFunctions.h>
#include <Utilities/type_index.h>

class MessageDispatcher
{
private:
	typedef std::unique_ptr<MessageHandlerBase> HandlerPtr;
	typedef std::pair<size_t/*hash*/, HandlerPtr> HandlerPair;
	typedef std::vector<HandlerPair> EventHandlers;	 // one event and different publishers needs different handlers
	typedef std::unordered_map<std::type_index, EventHandlers> Handlers;
	Handlers m_handlers;

public:
	MessageDispatcher() {}
	~MessageDispatcher() {}

	template < class HandlerType, typename EventType>
	void RegisterHandler(HandlerType& i_instance, void (HandlerType::*member_function)(const EventType&), const std::string& i_publisher);

	template < class EventType >
	void UnregisterHandler(const std::string& i_publisher);

	__forceinline void UnregisterHandler(const std::type_index& i_type, const std::string& i_publisher)
	{
		if (m_handlers.empty())
			return;
		Handlers::iterator handlers_it = m_handlers.find(i_type);
		if (handlers_it != m_handlers.end())
		{
			const size_t hash = SDK::Utilities::hash_function(i_publisher);
			auto& handlers = handlers_it->second;
			auto handler_it = std::find_if(handlers.begin(), handlers.end(), [hash](const HandlerPair& h_p)
			{
				return h_p.first == hash;
			});
			if (handler_it != handlers.end())
				handlers.erase(handler_it);
		}
	}

	template <typename EventType>
	void HandleMessage(const EventType& i_event, const std::string& i_publisher);
};

//////////////////////////////////////////////////////////////////////////

template < typename HandlerType, typename EventType>
void MessageDispatcher::RegisterHandler(HandlerType& i_instance, void (HandlerType::*member_function)(const EventType&), const std::string& i_publisher)
{
	using FunctionHandler = MemberFunctionHandler<HandlerType, const EventType&>;
	auto p_handler = std::make_unique< FunctionHandler >(i_instance, member_function);
	const size_t hash = SDK::Utilities::hash_function(i_publisher);
	EventHandlers& handlers = m_handlers[typeid(EventType)];
	handlers.push_back(std::make_pair(hash, std::move(p_handler)));
}

template < typename EventType >
void MessageDispatcher::UnregisterHandler(const std::string& i_publisher)
{
	UnregisterHandler(typeid(EventType), i_publisher);
}

template < typename EventType >
void MessageDispatcher::HandleMessage(const EventType& i_event, const std::string& i_publisher)
{
	Handlers::iterator handlers_it = m_handlers.find(std::type_index(typeid(i_event)));
	if (handlers_it != m_handlers.end())
	{
		const size_t hash = SDK::Utilities::hash_function(i_publisher);
		auto& handlers = handlers_it->second;
		auto handler_it = std::find_if(handlers.begin(), handlers.end(), [hash](const HandlerPair& h_p)
		{
			return h_p.first == hash;
		});
		if (handler_it != handlers.end())
			handler_it->second->ExecuteHandler(i_event);
	}

}