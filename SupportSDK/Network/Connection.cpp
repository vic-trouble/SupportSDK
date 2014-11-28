#include "stdafx.h"

#include "Connection.h"

#include "Address.h"
#include "Packet.h"
#include "SocketImpl.h"
#include "Utilities.h"

#include <Utilities/ConversionsUtilities.h>

namespace Network
  {

  Connection::Connection()
    : m_protocol_id(0)
    , m_timeout(0)
    , m_mode(ConnectionMode::None)
    , m_running(false)
    , m_timeout_accumulator(0)
    , m_packet_recieved_id(0)
    , m_packet_sent_id(0)
    , mp_socket(new SocketImpl())
    {
    _ClearState();
    }

  Connection::Connection(unsigned int i_protocol_id, float i_timeout)
    : m_protocol_id(i_protocol_id)
    , m_timeout(i_timeout)
    , m_mode(ConnectionMode::None)
    , m_running(false)
    , m_timeout_accumulator(0)
    , m_packet_recieved_id(0)
    , m_packet_sent_id(0)
    {
    _ClearState();
    }

  Connection::~Connection()
    {
    if (m_running)
      Stop();
    }

  bool Connection::Start(int i_port)
    {
    assert(!m_running);
    printf("<Network> start connection on port %d\n", i_port);
    if (!mp_socket->Open(i_port))
      return false;
    m_running = true;
    m_own_address = Utilities::GetDeviceAddress(i_port);
    return true;
    }

  void Connection::Stop()
    {
    assert(m_running);
    printf("<Network> stop connection\n");
    _ClearState();
    mp_socket->Close();
    m_running = false;
    }

  void Connection::Listen()
    {
    printf("<Network> server listening for connection\n");
    _ClearState();
    m_mode = ConnectionMode::Server;
    m_state = ConnectionState::Listening;
    }

  void Connection::Connect(const Address& i_address)
    {
    printf("<Network> client connecting to %d.%d.%d.%d:%d\n",
      i_address.GetA(), i_address.GetB(), i_address.GetC(), i_address.GetD(), i_address.GetPort());
    _ClearState();
    m_mode = ConnectionMode::Client;
    m_state = ConnectionState::Connecting;
    m_connection_address = i_address;
    }

  void Connection::Update(float i_deltaTime)
    {
    assert(m_running);
    m_timeout_accumulator += i_deltaTime;
    if (m_state == ConnectionState::Connecting)
      {
      if (m_timeout_accumulator > m_timeout)
        {
        printf("<Network> connect timed out\n");
        _ClearState();
        m_state = ConnectionState::ConnectFail;
        }
      }
    }

  bool Connection::SendPacket(const Packet& i_packet)
    {
    return SendPacket(i_packet.mp_data, i_packet.m_size);
    }

  bool Connection::SendPacket(const void* ip_data, size_t i_size)
    {
    assert(m_running);
    if (m_connection_address.GetAddress() == 0)
      return false;
    // TODO should cache packet buffer
    std::unique_ptr<unsigned char[]> p_packet(new unsigned char[i_size + 2 * sizeof(int)]);
    p_packet[0] = static_cast<unsigned char>(m_protocol_id >> 24);
    p_packet[1] = static_cast<unsigned char>((m_protocol_id >> 16) & 0xFF);
    p_packet[2] = static_cast<unsigned char>((m_protocol_id >> 8) & 0xFF);
    p_packet[3] = static_cast<unsigned char>((m_protocol_id)& 0xFF);
    //write packet id
    SDK::Utilities::ConvertToChar(&p_packet[4], ++m_packet_sent_id);
    SDK::Utilities::ConvertToChar(&p_packet[8], ip_data, i_size);
    return mp_socket->Send(m_connection_address, p_packet.get(), i_size + 2 * sizeof(int));
    }

  size_t Connection::ReceivePacket(void* ip_data, size_t i_size)
    {
    assert(m_running);
    std::unique_ptr<unsigned char[]> p_packet(new unsigned char[i_size + 2 * sizeof(int)]);
    Address sender;
    size_t bytes_read = mp_socket->Receive(sender, p_packet.get(), i_size + 4);
    if (bytes_read == 0)
      return 0;
    if (bytes_read <= 8)
      {
      printf("<Network> Read less than 8 bytes - seems to be an error");
      return 0;
      }
    if (p_packet[0] != (unsigned char)(m_protocol_id >> 24) ||
      p_packet[1] != (unsigned char)((m_protocol_id >> 16) & 0xFF) ||
      p_packet[2] != (unsigned char)((m_protocol_id >> 8) & 0xFF) ||
      p_packet[3] != (unsigned char)(m_protocol_id & 0xFF))
      {
      return 0;
      }

    int previous_packet_id = SDK::Utilities::ConvertFromChar<int>(&p_packet[4]);
    if (m_packet_recieved_id != previous_packet_id + 1)
      printf("<Network> Packet id is not correct\n");

    if (m_mode == ConnectionMode::Server && !IsConnected())
      {
      printf("<Network> server accepts connection from client %d.%d.%d.%d:%d\n",
        sender.GetA(), sender.GetB(), sender.GetC(), sender.GetD(), sender.GetPort());
      m_state = ConnectionState::Connected;
      m_connection_address = sender;
      }
    if (sender == m_connection_address)
      {
      if (m_mode == ConnectionMode::Client && m_state == ConnectionState::Connecting)
        {
        printf("<Network> client completes connection with server\n");
        m_state = ConnectionState::Connected;
        }
      m_timeout_accumulator = 0.0f;
      memcpy(ip_data, &p_packet[8], i_size - 8);
      return bytes_read - 8;
      }
    return 0;
    }

  void	Connection::Disconnect()
    {
    _ClearState();
    }

  void Connection::_ClearState()
    {
    m_mode = ConnectionMode::None;
    m_state = ConnectionState::Disconnected;
    m_timeout_accumulator = 0.0f;
    m_connection_address = Address();
    m_packet_recieved_id = -1;
    m_packet_sent_id = -1;
    mp_socket->Close();
    }

  } // Network