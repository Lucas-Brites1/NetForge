#include "HttpRequest.hpp"

HttpRequest::HttpRequest()
    : m_method(HTTPMethod::UNKNOWN), 
      m_uri(""),                     
      m_httpVersion(""),
      m_headers(),                
      m_body()                       
{}

void HttpRequest::setMethod(HTTPMethod method) { this->m_method = method; }
void HttpRequest::setUri(Uri& uri) { this->m_uri = uri; }
void HttpRequest::setVersion(const std::string& httpVersion) { this->m_httpVersion = httpVersion; }
void HttpRequest::setHeader(const std::string& key, const std::string& value) { this->m_headers[key] = value; }
void HttpRequest::setBody(const char* data, std::size_t data_len) { this->m_body.clear(); this->m_body.append(data, data_len); }
void HttpRequest::setBody(const std::string& data) { this->m_body.clear(); this->m_body.append(data.data(), data.length()); }
