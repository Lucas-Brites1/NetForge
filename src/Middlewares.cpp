#include "Middlewares.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "Routes.hpp"

void Middlewares::add(HTTPMethod method, const std::string& routes_to_bind, HttpHandler h_func)
{
  this->m_middlewares[method][routes_to_bind].push_back(h_func);
}

void Middlewares::use(HttpHandler h_func)
{
  this->m_global_middlewares.push_back(h_func);
}

bool Middlewares::execute(const std::string& req_path, const HTTPMethod req_method, const HttpRequest& http_req, HttpResponse& http_res)
{
  for (const auto& handler : m_global_middlewares) handler(http_req, http_res);

  auto method_it = m_middlewares.find(req_method);
  if (method_it == m_middlewares.end()) { return false; }
  
  auto route_it = method_it->second.find(req_path);
  if (route_it == method_it->second.end()) { return false; }

  for (const auto& handler : route_it->second) handler(http_req, http_res);
  return true;
}
