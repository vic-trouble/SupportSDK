#ifndef __GAMECORE_MESSAGEHANDLERBASE_H__
#define __GAMECORE_MESSAGEHANDLERBASE_H__

#include "../PatternsAPI.h"
#include "../Event.h"

class MessageHandlerBase
  {
  public:
	  virtual ~MessageHandlerBase() {}
	  virtual void ExecuteHandler(const Event& i_event) = 0;
  };

#endif