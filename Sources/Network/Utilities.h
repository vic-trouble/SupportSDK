#ifndef __UTILITILES_H__
#define __UTILITILES_H__

#include "NetworkAPI.h"

#include "Net.h"

namespace Network
  {

  class Address;

  namespace Utilities
    {

    Address NETWORK_API GetDeviceAddress(unsigned short i_port);

    bool    NETWORK_API InitializeSockets();
    void    NETWORK_API ShutdownSockets();

    } // Utilities

  } // Network

#endif