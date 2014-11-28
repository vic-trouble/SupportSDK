#pragma once

#include "NetworkAPI.h"

namespace Network
  {
  class Connection;
  struct Packet;

  class PacketProvicer
    {
    private:
      std::unique_ptr<unsigned char[]>  mp_buffer;
      size_t                            m_buffer_size;

      Connection&                       m_connection;

    public:
      NETWORK_API               PacketProvicer(Connection& i_connection);
      NETWORK_API               ~PacketProvicer();

      NETWORK_API Packet        GetNextPacket() const;
      Connection&               GetConnection();
    };

  //////////////////////////////////////////////////////////////////////////

  inline Connection& PacketProvicer::GetConnection()
    {
    return m_connection;
    }

  } // Network