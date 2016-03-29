#ifndef __DEFAULTNETWORKCONTROLLER_H__
#define __DEFAULTNETWORKCONTROLLER_H__

#include "INetworkController.h"

namespace Network
  {

  class DefaultNetworkController : public INetworkController
    {
    public:
      virtual ~DefaultNetworkController();

      virtual bool Initialize() override;
      virtual void Reset() override;
    };

  } // Network

#endif