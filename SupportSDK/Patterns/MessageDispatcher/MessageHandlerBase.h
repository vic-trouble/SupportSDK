#ifndef __GAMECORE_MESSAGEHANDLERBASE_H__
#define __GAMECORE_MESSAGEHANDLERBASE_H__

#include "../PatternsAPI.h"
#include "../Event.h"

template <typename EventBase = Event>
class MessageHandlerBase
  {
  public:
	  virtual ~MessageHandlerBase() {}
	  virtual void ExecuteHandler(const EventBase& i_event) = 0;
  };

#endif