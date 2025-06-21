#include "Routes.hpp"
#include <string>
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"

void Routes::add(HTTPMethod method, const std::string& url, HttpHandler h_func)
{
  this->m_routes[method][url] = h_func;
}

bool Routes::dispatch(HTTPMethod current_method, const std::string& current_url, const HttpRequest& req, HttpResponse& res) 
{
  auto method_it = m_routes.find(current_method);
  if (method_it == m_routes.end()) 
  {
    // Method not found...
    return false;
  }

  const auto& handlers_for_method = method_it->second;
  auto handler_it = handlers_for_method.find(current_url);
  if (handler_it == handlers_for_method.end())
  {
    // Route not found...
    return false;
  }

  try {
    handler_it->second(req, res);
    return true;
  } catch (const std::exception& e) {
    // Error trying to execute handler function
    return false;
  } catch (...) { 
    // Unknwon error;
    return false;
  }
}
