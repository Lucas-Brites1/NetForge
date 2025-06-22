#ifndef SOCKET_H
#define SOCKET_H

#include "Types.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <unistd.h>
#include <cstring>
#include <stdexcept>
#include "Buffer.hpp"

constexpr int BACKLOG_SIZE = 128;

class Socket
{
  private:
    int socket_fd;
    int addr_family; // IPv6 (AF_INET6) , IPv4 (AF_INET) , AF_UNIX
    int socket_type; // TCP (SOCK_STREAM) , UDP (SOCK_DGRAM), SOCK_RAW
                     //
    struct sockaddr_in socketcfg;

  public:
    Socket(int family, int type) : addr_family(family), socket_type(type)
    {
      socket_fd = socket(addr_family, type, 0);
      if (socket_fd == -1) 
      {
        throw std::runtime_error("Error while trying to create socket.");
      }
      std::memset(&socketcfg, 0, sizeof(socketcfg));
    }

    Socket(int existing_fd, int family, int type) : socket_fd(existing_fd), addr_family(family), socket_type(type)
    {
      if (socket_fd == -1)
      {
        throw std::runtime_error("Error: Invalid existing socket file descriptor.");
      }
      std::memset(&socketcfg, 0, sizeof(socketcfg));
    }

    Socket(Socket&& other) noexcept : socket_fd(other.socket_fd), addr_family(other.addr_family), socket_type(other.socket_type) { other.socket_fd = -1; }
    Socket& operator=(Socket&& other) noexcept 
    {
      if (this != &other)
      {
        if (socket_fd != -1) { close(socket_fd); }

        socket_fd = other.socket_fd;
        addr_family = other.addr_family;
        socket_type = other.socket_type;

        other.socket_fd = -1;
      }
      return *this;
    }

    Socket(const Socket&) = delete;
    Socket& operator=(const Socket&) = delete;

    ~Socket()
    {
      if(socket_fd != -1) 
      {
        close(socket_fd);
        socket_fd = -1;
      }
    }

    //Socket(const Socket&) = delete;
    //Socket& operator=(const Socket&) = delete;
    
    void bindSocket(const std::string& ip_address,  u16 port);
    void listenSocket();
    void connectSocket(const std::string& ip_address, u16 port);
    Socket acceptConnection();

    ssize_t receiveData(Buffer& inputBuffer);
    ssize_t sendData(Buffer& outputBuffer);

    //Getters
    int getSocketFd()   const { return socket_fd; }
    int getAddrFamily() const { return addr_family; }
    int getSocketType() const { return socket_type; }
};


#endif // !SOCKET
