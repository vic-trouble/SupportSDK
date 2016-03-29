#ifndef __ADDRESS_H__
#define __ADDRESS_H__

#include "NetworkAPI.h"

namespace Network
{

  class Address
    {
    public:

      NETWORK_API Address();
      NETWORK_API Address(unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned short port);
      NETWORK_API Address(unsigned int address, unsigned short port);

      NETWORK_API ~Address();

      NETWORK_API unsigned int GetAddress() const;

      NETWORK_API unsigned char GetA() const;
      NETWORK_API unsigned char GetB() const;
      NETWORK_API unsigned char GetC() const;
      NETWORK_API unsigned char GetD() const;

      NETWORK_API unsigned short GetPort() const;

      NETWORK_API bool operator == (const Address & other) const;
      NETWORK_API bool operator != (const Address & other) const;
      NETWORK_API bool operator < (const Address & other) const;

    private:
      unsigned int m_address;
      unsigned short m_port;
    };

} // Network

#endif