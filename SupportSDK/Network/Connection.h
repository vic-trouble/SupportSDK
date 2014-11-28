#ifndef __CONNECTION_H__
#define __CONNECTION_H__

#include "NetworkAPI.h"

#include "Address.h"

namespace Network
  {

  struct Packet;
  class ISocket;

  class Connection
    {
    public:
      enum class ConnectionMode
        {
        None,
        Client,
        Server
        };

    private:
      enum class ConnectionState
        {
        Disconnected,
        Listening,
        Connecting,
        ConnectFail,
        Connected
        };

      unsigned int      m_protocol_id;
      float             m_timeout;
      bool              m_running;
      float             m_timeout_accumulator;
      int               m_packet_recieved_id;
      int               m_packet_sent_id;
      ConnectionMode    m_mode;
      ConnectionState   m_state;

      typedef std::unique_ptr<ISocket> SocketPtr;
      SocketPtr         mp_socket;
      Address           m_connection_address;

      Address           m_own_address;

    private:
      void _ClearState();

    public:
      Connection();
      Connection(unsigned int i_protocol_id, float i_timeout);
      ~Connection();

      void            SetProtocolId(unsigned int i_protocol_id);
      void            SetTimeout(float i_timeout);

      bool            Start(int i_port);
      void            Stop();
      void            Listen();

      void            Update(float i_delta_time);
      void            Connect(const Address& i_address);
      void            Disconnect();

      bool            IsConnecting() const;
      bool            IsConnected() const;
      bool            IsListening() const;

      bool            ConnectFailed() const;
      ConnectionMode  GetMode() const;
      Address         GetConnectionAddress() const;
      Address         GetOwnAddress() const;

      bool            SendPacket(const Packet& i_packet);
      bool            SendPacket(const void* ip_data, size_t i_size);
      size_t          ReceivePacket(void* ip_data, size_t i_size);
    };

  inline void Connection::SetProtocolId(unsigned int i_protocol_id)
    {
    m_protocol_id = i_protocol_id;
    }

  inline void Connection::SetTimeout(float i_timeout)
    {
    m_timeout = i_timeout;
    }

  inline bool Connection::IsConnecting() const
    {
    return m_state == ConnectionState::Connecting;
    }

  inline bool Connection::ConnectFailed() const
    {
    return m_state == ConnectionState::ConnectFail;
    }

  inline bool Connection::IsConnected() const
    {
    return m_state == ConnectionState::Connected;
    }

  inline bool Connection::IsListening() const
    {
    return m_state == ConnectionState::Listening;
    }

  inline Connection::ConnectionMode Connection::GetMode() const
    {
    return m_mode;
    }

  inline Address Connection::GetConnectionAddress() const
    {
    return m_connection_address;
    }

  inline Address Connection::GetOwnAddress() const
    {
    return m_own_address;
    }

  } // Network

#endif