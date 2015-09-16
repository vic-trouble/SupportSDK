#include "stdafx.h"

#include "MessageDispatcher.h"

#include "Event.h"

MessageDispatcher::MessageDispatcher()
  {    }

MessageDispatcher::~MessageDispatcher()
  {    }

void MessageDispatcher::HandleMessage(const Event& i_event)
  {
  Handlers::iterator it = m_handlers.find(typeid(i_event));
  if(it != m_handlers.end())
    it->second->ExecuteHandler(i_event);
  }