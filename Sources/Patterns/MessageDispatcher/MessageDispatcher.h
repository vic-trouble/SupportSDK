#pragma once

#include "../PatternsAPI.h"

#include "MessageHandlerBase.h"
#include "MemberFunctionHandler.h"

#include <unordered_map>

#include <Utilities/ConstString.h>
#include <Utilities/HashFunctions.h>
#include <Utilities/type_index.h>

template <typename EventBase = Event>
class MessageDispatcherBase
{
private:
	typedef std::unique_ptr<MessageHandlerBase<EventBase>> HandlerPtr;
	typedef std::pair<size_t/*hash*/, HandlerPtr> HandlerPair;
	typedef std::vector<HandlerPair> EventHandlers;	 // one event and different publishers needs different handlers
	typedef std::pair<size_t/*type_hash*/, EventHandlers> HandlersPair;
	typedef std::vector<HandlersPair> Handlers;
	Handlers m_handlers;

public:
	MessageDispatcherBase() {}
	~MessageDispatcherBase() {}

	template < class HandlerType, typename EventType>
	void RegisterHandler(HandlerType& i_instance, void (HandlerType::*member_function)(const EventType&))
	{
		RegisterHandler<HandlerType, EventType>(i_instance, member_function, std::string());
	}

	template < class HandlerType, typename EventType>
	void RegisterHandler(HandlerType& i_instance, void (HandlerType::*member_function)(const EventType&), const std::string& i_publisher);

	template < class EventType >
	void UnregisterHandler(const std::string& i_publisher);

	void UnregisterHandler(const size_t i_event_type_hash, const std::string& i_publisher)
	{
		if (m_handlers.empty())
			return;
		auto handlers_it = std::find_if(m_handlers.begin(), m_handlers.end(), [i_event_type_hash](const HandlersPair& handlers)
		{
			return handlers.first == i_event_type_hash;
		});
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

template <typename EventBase>
template < typename HandlerType, typename EventType>
void MessageDispatcherBase<EventBase>::RegisterHandler(HandlerType& i_instance, void (HandlerType::*member_function)(const EventType&), const std::string& i_publisher)
{
	static const size_t event_type_hash = typeid(EventType).hash_code();
	using FunctionHandler = MemberFunctionHandler<HandlerType, const EventType&, EventBase>;
	// find list of handlers for specific event
	auto it = std::find_if(m_handlers.begin(), m_handlers.end(), [](const HandlersPair& handlers)
	{
		return handlers.first == event_type_hash;
	});
	if (it == m_handlers.end())
	{
		m_handlers.push_back(std::make_pair(event_type_hash, EventHandlers()));
		// iterator for last element
		it = m_handlers.end();
		--it;
	}

	auto p_handler = std::make_unique< FunctionHandler >(i_instance, member_function);
	const size_t hash = SDK::Utilities::hash_function(i_publisher);
	it->second.push_back(std::make_pair(hash, std::move(p_handler)));
}

template <typename EventBase>
template < typename EventType >
void MessageDispatcherBase<EventBase>::UnregisterHandler(const std::string& i_publisher)
{
	static const size_t event_type_hash = typeid(EventType).hash_code();
	UnregisterHandler(event_type_hash, i_publisher);
}

template <typename EventBase>
template < typename EventType >
void MessageDispatcherBase<EventBase>::HandleMessage(const EventType& i_event, const std::string& i_publisher)
{
	static const size_t event_type_hash = typeid(EventType).hash_code();
	static const size_t empty_string_hash = SDK::Utilities::hash_function(std::string());
	auto handlers_it = std::find_if(m_handlers.begin(), m_handlers.end(), [](const HandlersPair& handlers)
	{
		return handlers.first == event_type_hash;
	});
	if (handlers_it != m_handlers.end())
	{
		const size_t hash = SDK::Utilities::hash_function(i_publisher);
		auto& handlers = handlers_it->second;
		for (auto& handler : handlers)
		{
			if (handler.first == empty_string_hash || handler.first == hash)
				handler.second->ExecuteHandler(i_event);
		}
	}

}

using MessageDispatcher = MessageDispatcherBase<>;