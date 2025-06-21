#include "HttpResponse.hpp"
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <map>
#include <sstream>
#include <string>
#include <utility>

HttpResponse::HttpResponse(HttpStatus statusCode, const std::string& version, const std::string& contentType)
{
  this->m_statusCode = statusCode;
  this->m_reasonPhrase = this->generateResonPhrase(statusCode);
  this->m_httpVersion = version;
  this->m_headers.insert(std::make_pair("Content-Type", contentType));
}

std::string HttpResponse::generateResonPhrase(HttpStatus statuscode) const
{
 switch (statuscode)
 {
   case OK: { return "OK"; };
   case NotFound: { return "Not Found"; }
   default: { return "Internal Server Error"; }
 }
}

void HttpResponse::setStatusCode(HttpStatus status) { this->m_statusCode = status; this->generateResonPhrase(status); }
void HttpResponse::setHttpVersion(const std::string& version) { this->m_httpVersion = version; }
void HttpResponse::setReasonPhrase(const std::string& phrase) { this->m_reasonPhrase = phrase; }
void HttpResponse::setContentType(const std::string& type) { this->m_headers["Content-Type"] = type; }

void HttpResponse::setBody(const char* data, std::size_t data_len) 
{ 
  this->m_body.clear();
  this->m_body.append(data, data_len);
  addHeader("Content-Length", std::to_string(m_body.size()));
}

void HttpResponse::setBody(const std::string& data)
{
  this->m_body.clear();
  this->setBody(data.data(), data.length());
}

void HttpResponse::addHeader(const std::string& key, const std::string& value)
{
  this->m_headers.insert(std::make_pair(key, value));
}

const std::string HttpResponse::toString() const
{
  std::string full_response = "";
  
  full_response += m_httpVersion + " ";
  full_response += std::to_string(static_cast<u16>(m_statusCode)) + " ";
  full_response += getReasonPhrase() + "\r\n";
 
  if (m_headers.find("Date") == m_headers.end()) 
  {
        auto now = std::chrono::system_clock::now();
        std::time_t now_c = std::chrono::system_clock::to_time_t(now);
        std::stringstream ss;
        ss << std::put_time(std::gmtime(&now_c), "%a, %d %b %Y %H:%M:%S GMT");
        full_response += "Date: " + ss.str() + "\r\n";
  }

  if (m_headers.find("Server") == m_headers.end())
  {
    full_response += "Server: NetForge/1.0\r\n";
  }

  if (m_body.size() > 0 && m_headers.find("Content-Length") == m_headers.end())
  {
    full_response += "Content-Length: " + std::to_string(m_body.size()) + "\r\n";
  }
  
  for (const auto& pair : m_headers)
  {
    full_response += pair.first + ": " + pair.second + "\r\n";
  }

  full_response += "\r\n";
 
  if (m_body.size() > 0)
  {
    full_response.append(m_body.data(), m_body.size());
  }

  return full_response;
}
