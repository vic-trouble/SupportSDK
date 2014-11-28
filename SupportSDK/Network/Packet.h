#ifndef __PACKET_H__
#define __PACKET_H__

#include "PacketType.h"

namespace Network
  {
  
  struct Packet
    {
    const PacketType  m_packet;
    const size_t      m_size;
    const void*       mp_data;

    mutable size_t    m_current_offset;

    Packet (PacketType i_packet, size_t i_size, const void* ip_data)
      : m_packet(i_packet)
      , m_size(i_size)
      , mp_data(ip_data)
      , m_current_offset(0)
      {      }

    Packet ()
      : m_packet(PacketType::PT_NO_PACKETS)
      , m_size(0)
      , mp_data(nullptr)
      , m_current_offset(0)
      {      }

    bool operator () ()
      {
      return PacketType::PT_NO_PACKETS != m_packet;
      }

    void* GetPointerToCurrent()
      {
      return const_cast<void*>(GetPointerToCurrentConst());
      }

    const void* GetPointerToCurrentConst() const
      {
      const unsigned char* p_data = static_cast<const unsigned char*>(mp_data);
      const void* p_offset_data = p_data + m_current_offset;
      return p_offset_data;
      }

    void IncreaseOffset (size_t i_offset) const
      {
      m_current_offset += i_offset;
      if (m_current_offset > m_size)
        throw std::runtime_error("[Packet] Offset is more than size");
      }
    };

  } // Network

#endif