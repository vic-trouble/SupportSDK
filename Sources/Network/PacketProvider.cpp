#include "stdafx.h"

#include "PacketProvider.h"
#include "Packet.h"

#include <Utilities/ConversionsUtilities.h>

namespace Network
  {

  PacketProvicer::PacketProvicer(Connection& i_connection)
    : mp_buffer(new unsigned char[255])
    , m_connection(i_connection)
    , m_buffer_size(255)
    {    }

  PacketProvicer::~PacketProvicer()
    {
    mp_buffer.reset();
    }

  Packet PacketProvicer::GetNextPacket() const
    {
    int bytes_read = 0;// m_connection.ReceivePacket(&mp_buffer[0], m_buffer_size);

    if (bytes_read == 0)
      return Packet();

    PacketType packet_type = SDK::Utilities::ConvertFromChar<PacketType> ( mp_buffer.get() );
    bytes_read -= sizeof(PacketType);

    assert (bytes_read >= 0);

    size_t packet_data_size = bytes_read;
    void* p_data = bytes_read != 0 ? &mp_buffer[sizeof(PacketType)] : nullptr;

    return Packet(packet_type, packet_data_size, p_data);
    }

  } // Network