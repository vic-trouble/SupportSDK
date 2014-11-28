#ifndef __ISOCKET_H__
#define __ISOCKET_H__

namespace Network
  {

  class Address;

  class ISocket
    {
    protected:
      int m_socket;

    public:
      ISocket() : m_socket(0) {}
      virtual ~ISocket() {}

      virtual bool    Open(unsigned short i_port) = 0;
      virtual void    Close() = 0;
      bool            IsOpen() const;

      virtual bool    Send(const Address& i_destination, const void* ip_data, size_t i_size) = 0;
      virtual size_t  Receive(Address& o_sender, void* op_data, size_t i_size) = 0;
    };

  inline bool ISocket::IsOpen() const
    {
    return m_socket != 0;
    }

  } // Network

#endif