#include "stdafx.h"

#include "SocketImpl.h"

#include "Utilities.h"

#if PLATFORM == PLATFORM_WINDOWS

#include <winsock2.h>
#pragma comment( lib, "wsock32.lib" )

#elif PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX

#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>

#else

#error unknown platform!

#endif

namespace Network
  {

  SocketImpl::SocketImpl()
    {
    }

  SocketImpl::~SocketImpl()
    {
    Close();
    }

  bool SocketImpl::Open(unsigned short i_port)
    {
    assert(!IsOpen());

    // create m_socket
    m_socket = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (m_socket <= 0)
      {
      printf("<Network> failed to create m_socket\n");
      m_socket = 0;
      return false;
      }

    // bind to port
    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons((unsigned short)i_port);
    // TODO get server port via getsockport
    //http://msdn.microsoft.com/en-us/library/ms738544(v=vs.85).aspx
    if (bind(m_socket, reinterpret_cast<const sockaddr*>(&address), sizeof(sockaddr_in)) < 0)
      {
      printf("<Network> failed to bind m_socket\n");
      Close();
      return false;
      }

    // set non-blocking io

#if PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX

    int nonBlocking = 1;
    if (fcntl(m_socket, F_SETFL, O_NONBLOCK, nonBlocking) == -1)
      {
      printf("<Network> failed to set non-blocking m_socket\n");
      Close();
      return false;
      }

#elif PLATFORM == PLATFORM_WINDOWS

    DWORD nonBlocking = 1;
    if (ioctlsocket(m_socket, FIONBIO, &nonBlocking) != 0)
      {
      printf("failed to set non-blocking m_socket\n");
      Close();
      return false;
      }

#endif

    return true;
    }

  void SocketImpl::Close()
    {
    if (m_socket != 0)
      {
#if PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX
      close(m_socket);
#elif PLATFORM == PLATFORM_WINDOWS
      closesocket(m_socket);
#endif
      m_socket = 0;
      }
    }

  bool SocketImpl::Send(const Address& i_destination, const void* ip_data, size_t i_size)
    {
    assert(ip_data);
    assert(i_size > 0);

    if (m_socket == 0)
      return false;

    assert(i_destination.GetAddress() != 0);
    assert(i_destination.GetPort() != 0);

    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(i_destination.GetAddress());
    address.sin_port = htons((unsigned short)i_destination.GetPort());

    int sent_bytes = sendto(m_socket, (const char*)ip_data, i_size, 0, (sockaddr*)&address, sizeof(sockaddr_in));

    if (-1 == sent_bytes)
      {
      int errsv = errno;
      printf("Errno: %d\n", errsv);
      }
    bool all_sent = sent_bytes == i_size;
    if (all_sent == false)
      printf("<Network> Sent/size: %x/%x\n", sent_bytes, i_size);
    return all_sent;
    }

  size_t SocketImpl::Receive(Address& o_sender, void* ip_data, size_t i_size)
    {
    assert(ip_data);
    assert(i_size > 0);

    if (m_socket == 0)
      return false;

#if PLATFORM == PLATFORM_WINDOWS
    typedef int socklen_t;
#endif

    sockaddr_in from;
    socklen_t fromLength = sizeof(from);

    int received_bytes = recvfrom(m_socket, (char*)ip_data, i_size, 0, (sockaddr*)&from, &fromLength);

    if (received_bytes <= 0)
      return 0;

    unsigned int address = ntohl(from.sin_addr.s_addr);
    unsigned short port = ntohs(from.sin_port);

    o_sender = Address(address, port);

    return received_bytes;
    }

  } // Network