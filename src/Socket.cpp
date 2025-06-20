#include "Socket.hpp"
#include <netinet/in.h>
#include <sys/socket.h>
#include <cstdio>
#include <cstring>
#include <stdexcept>
#include <arpa/inet.h>
#include <sys/types.h>
#include "Utils.hpp"

void Socket::bindSocket(const std::string& ip_address, u16 port)
{
  if (socket_fd == -1) 
  {
    throw std::runtime_error("Error: Socket must be initialized first.");
  }

  if (addr_family == AF_INET) // IPV4
  {
    socketcfg.sin_family = addr_family;
    socketcfg.sin_port = mHTONS(port);

    if (ip_address == "0.0.0.0" || ip_address == "INADDR_ANY")
    {
      socketcfg.sin_addr.s_addr = INADDR_ANY;
    }
    else if ( inet_pton(AF_INET, ip_address.c_str(), &(socketcfg.sin_addr)) <= 0 ) 
    {
      throw std::runtime_error("Error trying to convert ip_address to binary.");
    }
  }
  else if (addr_family == AF_INET6) 
  {
    // TO-DO
    throw std::runtime_error("IPv6 address family not yet implemented for bind.");
  }
  else
  {
    throw std::runtime_error("Address family not supported for connection.");
  }

  if ( bind(socket_fd, reinterpret_cast<struct sockaddr*>(&socketcfg), sizeof(socketcfg)) < 0)
  {
    std::perror("Socket bind failed.");
    throw std::runtime_error("Failed to bind address and port to socket.");
  };
};

void Socket::listenSocket()
{
  if (socket_fd == -1) 
  {
    throw std::runtime_error("Error: Socket must be initialized first.");
  }

  if ( listen(socket_fd, BACKLOG_SIZE) == -1 ) 
  {
    std::perror("Socket Listen error.");
    throw std::runtime_error("Failed to set socket to listen.");
  }
}

void Socket::connectSocket(const std::string& ip_address, u16 port)
{
  if (socket_fd == -1)
  {
    throw std::runtime_error("Error: Socket must be initialized first.");
  }

  if (addr_family == AF_INET)
  {
    socketcfg.sin_family = addr_family;
    socketcfg.sin_port = mHTONS(port);
    
    if ( inet_pton(AF_INET, ip_address.c_str(), &(socketcfg.sin_addr)) <= 0)
    {
      throw std::runtime_error("Error trying to convert IPv4 address for connection.");
    }
  }
  else if (addr_family == AF_INET6)
  {
    // TO-DO
    throw std::runtime_error("IPv6 address family not yet implemented for connect.");
  }
  else
  {
    throw std::runtime_error("Address family not supported for connection.");
  }

  if ( connect(socket_fd, reinterpret_cast<struct sockaddr*>(&socketcfg), sizeof(socketcfg)) == -1 )
  {
    std::perror("Socket connection failed.");
    throw std::runtime_error("Failed to connect to the specified address and port.");
  }
}

Socket Socket::acceptConnection()
{
  if (socket_fd == -1)
  {
    throw std::runtime_error("Error: Socket must be initialized and listening before accepting connections.");
  }

  struct sockaddr_in client_socket;
  socklen_t client_socket_len = sizeof(client_socket);
  std::memset(&client_socket, 0, sizeof(client_socket));

  int new_socket_fd = accept(socket_fd, reinterpret_cast<sockaddr*>(&client_socket), &client_socket_len); 
  if (new_socket_fd == -1)
  {
    std::perror("Socket accept failed.");
    throw std::runtime_error("Failed to accept new client connection.");
  }

  return Socket(new_socket_fd, client_socket.sin_family, socket_type);
}

ssize_t Socket::receiveData(Buffer& inputBuffer)
{
  if (socket_fd == -1)
  {
    throw std::runtime_error("Error: Socket must be initialized first before receiving data.");
  }

  ssize_t bytes_recv = recv(socket_fd, inputBuffer.write_ptr(), inputBuffer.availableSpace(), 0);
  if (bytes_recv == -1)
  {
    std::perror("Socket receiveData error");
    throw std::runtime_error("Failed to receive data from socket.");
  }
  else if (bytes_recv == 0) { return 0; }

  inputBuffer.advance_write_ptr(bytes_recv);
  return bytes_recv;
}

ssize_t Socket::sendData(Buffer& outputBuffer)
{
  if (socket_fd == -1)
  {
    throw std::runtime_error("Error: Socket must be initialized first before sending data.");
  }

  if (outputBuffer.size() == 0) return 0;

  ssize_t bytes_sent = send(socket_fd, outputBuffer.data(), outputBuffer.size(), 0);
  if (bytes_sent == -1)
  {
    std::perror("Socket sendData error");
    throw std::runtime_error("Failed to send data from socket.");
  }

  outputBuffer.advance_read_ptr(bytes_sent);
  return bytes_sent;
}
