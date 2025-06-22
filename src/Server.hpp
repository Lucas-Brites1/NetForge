#ifndef SERVER_H
#define SERVER_H

#include <sys/socket.h>
#include <vector>
#include "Client.hpp"
#include "HttpRequest.hpp"
#include "Middlewares.hpp"
#include "Routes.hpp"
#include "Socket.hpp"
#include "Types.hpp"

constexpr int DEFAULT_SOCK_FAMILY = AF_INET;    // IPV4 by default
constexpr int DEFAULT_SOCK_TYPE  = SOCK_STREAM; // TCP  by default

constexpr int MAX_ROUTES = 256;
constexpr int MAX_MIDDLEWARES = 128;

enum AddType { ROUTE=0x0A, MIDDLEWARE=0x0B };

class Server
{
  private:
    Socket                  serverSocket;
    std::vector<Client>     clientsConnections;
    Routes                  serverRoutes;
    Middlewares             serverMiddlewares;

    void process(Client& client);
    void h_NotFound(const HttpRequest& req, HttpResponse& res);

  public:
    Server(const std::string& ip_address, u16 port, int sock_family = DEFAULT_SOCK_FAMILY, int sock_type = DEFAULT_SOCK_TYPE);
    ~Server() {};
    
    void start();
    void add(AddType type, HTTPMethod method, const std::string& url, HttpHandler handler_func);
    void use(HttpHandler h_func); // use() to register global middlewares;
};


#endif // !
