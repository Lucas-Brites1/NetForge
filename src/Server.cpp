#include "Server.hpp"
#include <sys/socket.h>
#include <exception>
#include <iostream>
#include <stdexcept>
#include <vector>
#include "Client.hpp"
#include "HttpRequest.hpp"
#include "Socket.hpp"
#include "Utils.hpp"

Server::Server(const std::string& ip_address, u16 port, int sock_family, int sock_type) : serverSocket(sock_family, sock_type)
{
  try {
  serverSocket.bindSocket(ip_address, port);
  serverSocket.listenSocket();
  std::cout << "Server listening at: " << ip_address << ':' << port << '\n';
  } catch (const std::exception& e) {
    throw std::runtime_error("Failed to initialize server.");
  } 
}

void Server::start()
{
  while(true)
  {
    Socket newClientSocket = serverSocket.acceptConnection();
    clientsConnections.emplace_back(std::move(newClientSocket));
    Client& current_client = clientsConnections.back();
    HttpRequest http_req;
    HttpResponse http_res;

    current_client.readFromSocket();
    // readFromSocket -> parsing to HTTPREQUEST -> fill HTTPRESPONSE -> writeFromSocket
    parseHttpRequest(current_client.inputBuffer, http_req);
    std::cout << "Method: " << getMethodStr(http_req.getMethod()) << ", Endpoint: " << http_req.getUri().getRawUri() << ", Version: " << http_req.getVersion() << '\n';
    std::cout << "REQUEST HEADERS:\n";
    for (const auto& header : http_req.getHeaders()) 
    {
      std::cout << "  " << header.first << ": " << header.second << '\n';
    }
    std::cout << "--- END HEADERS ---\n";
    this->serverRoutes.dispatch(http_req.getMethod(), http_req.getUri().getRawUri(), http_req, http_res);

    /*const std::string http_response =
                            "HTTP/1.1 200 OK\r\n" // Status line
                            "Content-Type: text/html\r\n" // Type of content
                            "Content-Length: 12\r\n" // Length of "Hello World"
                            "\r\n" // Empty line to signal end of headers
                            "Hello World\n"; // Body of the response
                                                    */

    std::string final_http_response_str = http_res.toString();
    current_client.outputBuffer.append(final_http_response_str.data(), final_http_response_str.length());
    ssize_t bytes_sent = current_client.writeFromSocket();

    std::cout << "Sent " << bytes_sent << " bytes to client" << '\n';
    std::cout << current_client.outputBuffer.data() << '\n';

    std::cout << "Request: " << current_client.inputBuffer.data() << '\n';
  }
}

void Server::add(AddType type, HTTPMethod method, const std::string& url, HttpHandler handler_func)
{
  switch (type)
  {
    case ROUTE:
    {
      this->serverRoutes.add(method, url, handler_func);
      std::cout << "Rota '" << "GET" << " " << url << "' adicionada." << std::endl;
      break;
    }
    case MIDDLEWARE:
    {
      break;
    }
    default:
    {
      break;
    }
  }
}

