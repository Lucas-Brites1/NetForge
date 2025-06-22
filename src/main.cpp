#include <iostream>
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "Utils.hpp"
#include "Server.hpp"

int main(void)
{
  //Server *s = new Server("0.0.0.0", 8080);
  Server app("0.0.0.0", 7979);
  app.add(ROUTE, GET, "/hello", [](const HttpRequest& req, HttpResponse& res) {
       const std::string body_content = "<h1>Hello, World!</h1>\n";
      
       res.setBody(body_content);
       res.setHttpVersion("HTTP/1.1");
       res.setContentType("text/html");
       res.setStatusCode(OK);
      });

  app.use([](const HttpRequest& req, HttpResponse& res)
      {
        std::cout << '\n' << "Global Logger Middleware: " << '\n';
        std::cout << "[" << getMethodStr(req.getMethod()) << "] " << req.getUri().getPath() << '\n';
      });

  app.start();
}
