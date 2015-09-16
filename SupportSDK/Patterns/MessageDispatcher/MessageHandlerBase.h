#ifndef __GAMECORE_MESSAGEHANDLERBASE_H__
#define __GAMECORE_MESSAGEHANDLERBASE_H__

#include "PatternsAPI.h"

struct Event;

class MessageHandlerBase
  {
  private:
    virtual void HandleMessage(const Event& i_event) = 0;

  public:
	  virtual PATTERNS_API  ~MessageHandlerBase();
	  void PATTERNS_API     ExecuteHandler(const Event& i_event);
  };

#endif