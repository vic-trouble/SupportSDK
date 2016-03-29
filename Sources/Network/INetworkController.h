#ifndef __INETWORKCONTROLLER_H__
#define __INETWORKCONTROLLER_H__

namespace Network
  {

  class INetworkController
    {
    public:
      virtual ~INetworkController(){}

      virtual bool Initialize() = 0;
      virtual void Reset() = 0;
    };

  } // Network

#endif