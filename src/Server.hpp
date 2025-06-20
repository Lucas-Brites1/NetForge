#ifndef SERVER_H
#define SERVER_H

#include <sys/socket.h>
#include <exception>
#include <iostream>
#include <stdexcept>
#include <vector>
#include "Client.hpp"
#include "Socket.hpp"

constexpr int DEFAULT_SOCK_FAMILY = AF_INET;    // IPV4 by default
constexpr int DEFAULT_SOCK_TYPE  = SOCK_STREAM; // TCP  by default

class Server
{
  private:
    Socket                  serverSocket;
    std::vector<Client>     clientsConnections;
    //std::vector<Route>      routes;
    //std::vector<Middleware> middleware;

  public:
    Server(const std::string& ip_address, u16 port, int sock_family = DEFAULT_SOCK_FAMILY, int sock_type = DEFAULT_SOCK_TYPE) : serverSocket(sock_family, sock_type)
    {
      try 
      {
      serverSocket.bindSocket(ip_address, port);
      serverSocket.listenSocket();
      std::cout << "Server listening at: " << ip_address << ':' << port << '\n';
      }
      catch (const std::exception& e)
      {
        throw std::runtime_error("Failed to initialize server.");
      }

      while(true)
      {
        Socket newClientSocket = serverSocket.acceptConnection();
        clientsConnections.emplace_back(std::move(newClientSocket));
        Client& current_client = clientsConnections.back();

        const std::string http_response =
                "HTTP/1.1 200 OK\r\n" // Status line
                "Content-Type: text/html\r\n" // Type of content
                "Content-Length: 12\r\n" // Length of "Hello World"
                "\r\n" // Empty line to signal end of headers
                "Hello World\n"; // Body of the response
                                 //
        current_client.outputBuffer.append(http_response.data(), http_response.length());
        ssize_t bytes_sent = current_client.writeFromSocket();
        std::cout << "Sent " << bytes_sent << " bytes to client" << '\n';
        clientsConnections.pop_back();
      }
    }

    ~Server() {};
};


#endif // !
