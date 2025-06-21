#ifndef CLIENT_H
#define CLIENT_H

#include <sys/types.h>
#include "Types.hpp"
#include "Buffer.hpp"
#include "Socket.hpp"

class Client
{
  private:
    Socket conn_socket;

  public:
    Buffer inputBuffer;
    Buffer outputBuffer;

    Client(Socket&& socket) noexcept : conn_socket(std::move(socket)), inputBuffer(), outputBuffer() {};
    Client(int socket_family, int socket_type) : conn_socket(socket_family, socket_type), inputBuffer(), outputBuffer() {};
    Client(Client&& other) noexcept : 
      conn_socket(std::move(other.conn_socket)), inputBuffer(std::move(other.inputBuffer)), outputBuffer(std::move(other.outputBuffer)) {};

    Client(const Client&) = delete;
    Client& operator=(const Client&) = delete;

    void connectToServer(const std::string& ip, u16 port);
    ssize_t readFromSocket();
    ssize_t writeFromSocket();
};

#endif // !CLIENT_H
