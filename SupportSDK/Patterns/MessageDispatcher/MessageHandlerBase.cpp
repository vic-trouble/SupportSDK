#include "stdafx.h"

#include "MessageHandlerBase.h"

MessageHandlerBase::~MessageHandlerBase()
  {  }

void MessageHandlerBase::ExecuteHandler(const Event& i_event)
  {
  HandleMessage(i_event);
  }