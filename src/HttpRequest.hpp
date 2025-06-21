#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H
#include <map>
#include <string>
#include "Buffer.hpp"
#include "Uri.hpp"

enum HTTPMethod
{
  GET,
  POST,
  PUT,
  DELETE,
  PATCH,
  HEAD,
  OPTIONS,
  UNKNOWN
};

class HttpRequest
{
  public:
    HttpRequest();
    HttpRequest(HTTPMethod method, Uri& uri, const std::string& httpVersion) : m_method(UNKNOWN), m_uri(""), m_httpVersion(""), m_headers(), m_body() {}; 

    // getters
    const HTTPMethod getMethod() const { return this->m_method; }
    const Uri& getUri() const { return this->m_uri; }
    const std::string& getVersion() const { return this->m_httpVersion; }
    const std::map<std::string, std::string>& getHeaders() const { return this->m_headers; }
    const Buffer& getBody() const { return this->m_body; }
    const std::string& getHeader(const std::string& key) const 
    { 
      auto it = m_headers.find(key);
      if (it != m_headers.end())
      {
        return it->second;
      }

      static const std::string EMPTY_STRING = "";
      return EMPTY_STRING;
    }

    // setters
    void setMethod(HTTPMethod method);
    void setUri(Uri& uri);
    void setVersion(const std::string& httpVersion);
    void setHeader(const std::string& key, const std::string& value);
    void setBody(const char* data, std::size_t data_len);
    void setBody(const std::string& data);

  private:
    HTTPMethod m_method;
    Uri m_uri;
    std::string m_httpVersion;
    std::map<std::string, std::string> m_headers;
    Buffer m_body;
};

#endif // !HTTPREQUEST_H

