#ifndef MIDDLEWARE_H
#define MIDDLEWARE_H

#include <map>
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "Routes.hpp"

class Middlewares
{
  private:
    std::vector<HttpHandler> m_global_middlewares;
    
    std::map<HTTPMethod, std::map<std::string, std::vector<HttpHandler>>> m_middlewares;
    
  public:
    void use(HttpHandler h_func); // for global middlewares
    void add(HTTPMethod method, const std::string& routes_to_bind, HttpHandler h_func); 
    bool execute(const std::string& req_path, const HTTPMethod req_method, const HttpRequest& http_req, HttpResponse& http_res);
};

#endif // !MIDDLEWARE_H
