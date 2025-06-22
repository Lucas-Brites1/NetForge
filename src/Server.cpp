#include "Server.hpp"
#include <sys/socket.h>
#include <exception>
#include <iostream>
#include <ostream>
#include <stdexcept>
#include <vector>
#include "Client.hpp"
#include "HttpRequest.hpp"
#include "Routes.hpp"
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

void Server::process(Client& client)
{
  HttpRequest req;
  HttpResponse res;
  
  ssize_t bytes_read = client.readFromSocket();
  if (bytes_read <= 0) return;
  
  try 
  {
    parseHttpRequest(client.inputBuffer, req);

    serverMiddlewares.execute(req.getUri().getRawUri(), req.getMethod(), req, res);
    bool route_found = serverRoutes.dispatch(req.getMethod(), req.getUri().getPath(), req, res);
    
    if (!route_found)
    {
      Server::h_NotFound(req, res);
    }

    const std::string final_response_str = res.toString();
    client.outputBuffer.append(final_response_str.data(), final_response_str.length());
    
    ssize_t bytes_sent = client.writeFromSocket();
    std::cout << "Sent " << bytes_sent << " bytes." << '\n';

    client.inputBuffer.clear();
    client.outputBuffer.clear();
  } catch (const std::exception E)
  {
    std::cerr << "Error processing client request." << E.what() << std::endl;
    // 500 (Internal Server Error)
  }   
}

void Server::start()
{
  while(true)
  {
    Socket newClientSocket = serverSocket.acceptConnection();
    clientsConnections.emplace_back(std::move(newClientSocket));
    Client& client_in_vector = clientsConnections.back();
    Server::process(client_in_vector);
  }
}

void Server::add(AddType type, HTTPMethod method, const std::string& url, HttpHandler handler_func)
{
  switch (type)
  {
    case ROUTE:
    {
      this->serverRoutes.add(method, url, handler_func);
      break;
    }
    case MIDDLEWARE:
    {
      this->serverMiddlewares.add(method, url, handler_func);
      break;
    }
    default:
    {
      break;
    }
  }
}

void Server::use(HttpHandler h_func)
{
  this->serverMiddlewares.use(h_func);
}

void Server::h_NotFound(const HttpRequest& req, HttpResponse& res)
{
  const std::string body_content = "<h1>404 Not Found</h1><p>The requested resource was not found on this server.</p>";

  res.setBody(body_content);
  res.setStatusCode(HttpStatus::NotFound);
}
