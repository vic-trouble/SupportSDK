#ifndef __SOCKETIMPL_H__
#define __SOCKETIMPL_H__

#include "ISocket.h"

namespace Network
  {

  class SocketImpl : public ISocket
    {
    public:
      SocketImpl();
      virtual ~SocketImpl();

      virtual bool    Open(unsigned short i_port) override;
      virtual void    Close() override;

      virtual bool    Send(const Address& i_destination, const void* ip_data, size_t i_size) override;
      virtual size_t  Receive(Address& o_sender, void* op_data, size_t i_size) override;
    };

  } // Network

#endif