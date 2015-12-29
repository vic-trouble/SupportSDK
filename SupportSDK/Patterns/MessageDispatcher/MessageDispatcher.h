#pragma once

#include "../PatternsAPI.h"

#include "MessageHandlerBase.h"
#include "MemberFunctionHandler.h"

#include "type_index.h"

#include <unordered_map>

struct Event;

class MessageDispatcher
  {
  private:
    typedef std::unordered_map<std::type_index, std::unique_ptr<MessageHandlerBase>> Handlers;
    Handlers m_handlers;

  public:
    PATTERNS_API MessageDispatcher();
    PATTERNS_API ~MessageDispatcher();

    template < class HandlerType, class EventType >
    void RegisterHandler(HandlerType& i_instance, void (HandlerType::*member_function)(const EventType&));

    template < class EventType >
    void UnregisterHandler ();

	void PATTERNS_API HandleMessage(const Event& i_event);

  };

//////////////////////////////////////////////////////////////////////////

template < typename HandlerType, typename EventType >
void MessageDispatcher::RegisterHandler(HandlerType& i_instance, void (HandlerType::*member_function)(const EventType&))
  {
  m_handlers[typeid(EventType)].reset(new MemberFunctionHandler< HandlerType, EventType >(i_instance, member_function));
  }

template < typename EventType >
void MessageDispatcher::UnregisterHandler()
  {
  m_handlers.erase(typeid(EventType));
  }