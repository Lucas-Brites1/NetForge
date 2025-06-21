#ifndef ROUTE_H
#define ROUTE_H

#include <functional>
#include <map>
#include <string>
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"

using HttpHandler = std::function<void(const HttpRequest&,  HttpResponse&)>;

class Routes
{
  private:
    std::map<HTTPMethod, std::map<std::string, HttpHandler>> m_routes;

  public:
    void add(HTTPMethod method, const std::string& url, HttpHandler h_func);
    bool dispatch(HTTPMethod current_method, const std::string& current_url, const HttpRequest& req, HttpResponse& res);
};

#endif // !ROUTE_H
