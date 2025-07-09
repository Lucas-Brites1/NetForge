#include <iostream>
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "JsonValue.hpp"
#include "JSON.hpp"
#include "Utils.hpp"
#include "Server.hpp"

int main(void)
{
  //Server *s = new Server("0.0.0.0", 8080);
  Server app("0.0.0.0", 7979);
  app.add(ROUTE, GET, "/helloJSON", [](const HttpRequest& req, HttpResponse& res) {
       JsonValue JSONTest1 = JsonValue::Object({
           {"message", "Hello, World!"},
           {"version", 1.0}
           });
  
       res.setBody(JSONTest1.toString(0));
       res.setHttpVersion("HTTP/1.1");
       res.setContentType("application/json");
       res.setStatusCode(OK);
      });
  
  app.add(ROUTE, GET, "/hello", [](const HttpRequest& req, HttpResponse& res) {
      const std::string body_content = "<h1>Hello, World!</h1>\n";
      res.setBody(body_content);
      res.setHttpVersion("HTTP/1.1");
      res.setContentType("text/html");
      res.setStatusCode(OK);
      });

  app.add(ROUTE, GET, "/test", [](const HttpRequest& req, HttpResponse& res){
      JSON json_obj({
          {"id", 24009893},
          {"client", JsonValue::Object({
              {"name", "Lucas"},
              {"age", 23}
              })},
          {"message", "Hello, world!"}
      });
      json_obj.serialize();

      json_obj.Dump(2);
      res.setBody(json_obj.Data());
      std::cout << "JSON STRINGFIED: " << json_obj.Data() << '\n';
      res.setHttpVersion("HTTP/1.1");
      res.setContentType("application/json");
      res.setStatusCode(OK);
      });

  app.use([](const HttpRequest& req, HttpResponse& res)
      {
        std::cout << '\n' << "Global Logger Middleware: " << '\n';
        std::cout << "[" << getMethodStr(req.getMethod()) << "] " << req.getUri().getPath() << '\n';
      });

  app.start();
}
