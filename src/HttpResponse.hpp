#ifndef httpresponse_h
#define httpresponse_h
#include <map>
#include <string>
#include "Types.hpp"
#include "Buffer.hpp"

enum HttpStatus : u16
{
  OK = 200,
  NotFound = 404,
  InternalServerError = 500
};

class HttpResponse
{
  public:
    HttpResponse(HttpStatus statusCode = HttpStatus::OK, const std::string& version = "HTTP/1.1", const std::string& contentType = "text/html");
    
    void setStatusCode(HttpStatus status);
    void setHttpVersion(const std::string& version);
    void setReasonPhrase(const std::string& phrase);
    void setContentType(const std::string& type);
    void setBody(const char* data, std::size_t data_len);
    void setBody(const std::string& data);
    void addHeader(const std::string& key, const std::string& value);

    const HttpStatus getStatusCode() const { return m_statusCode; }
    const std::string& getHttpVersion() const { return m_httpVersion; }
    const std::string& getReasonPhrase() const { return m_reasonPhrase; }
    const std::string& getContentType() const { return m_headers.at("Content-Type"); }
    std::size_t getContentLength() const { return m_body.size(); }
    const Buffer& getBody() const { return m_body; }
    const std::string& getHeader(const std::string& key) const { return m_headers.at(key); }

    const std::string toString() const;

  private:
    // status line
    std::string  m_httpVersion;
    std::string  m_reasonPhrase;
    HttpStatus   m_statusCode;

    // headers
    std::map<std::string, std::string> m_headers;

    // Body
    Buffer       m_body;

    std::string generateResonPhrase(HttpStatus statuscode) const;
};

#endif // !httpresponse_h

