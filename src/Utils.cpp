#include "Utils.hpp"
#include <cstddef>
#include <sstream>
#include <string>
#include "Buffer.hpp"
#include "HttpRequest.hpp"
#include "Uri.hpp"

u16 mHTONS(u16 port)
{
  u16 mask_for_endianness_check = 0x0102;
  u8 *byte_ptr = (u8 *)&mask_for_endianness_check;

  // 0x01 - MSB
  // 0x02 - LSB
  // little-endian : 0x02, 0x01
  // big-endian    : 0x01, 0x02
  
  if (byte_ptr[0] == 0x02) // little endian
  {
    return (port << 8) | (port >> 8);
  }

  // big endian
  return port;
}

const std::string getMethodStr(HTTPMethod method)
{
  switch (method)
  {
    case GET: { return "GET"; }
    case PATCH: { return "PATCH"; }
    case POST: { return "POST"; }
    case PUT: { return "PUT"; }
    case DELETE: { return "DELETE"; }
    case HEAD: { return "HEAD"; }
    case UNKNOWN: { return "UNKNOWN"; }
    default: { return "UNKNOWN"; }
  }
}

HTTPMethod getMethodFromStr(const std::string& methodStr) {
    if (methodStr == "GET") {
        return HTTPMethod::GET;
    } else if (methodStr == "POST") {
        return HTTPMethod::POST;
    } else if (methodStr == "PUT") {
        return HTTPMethod::PUT;
    } else if (methodStr == "DELETE") {
        return HTTPMethod::DELETE;
    } else if (methodStr == "PATCH") {
        return HTTPMethod::PATCH;
    } else if (methodStr == "HEAD") {
        return HTTPMethod::HEAD;
    } else if (methodStr == "OPTIONS") {
        return HTTPMethod::OPTIONS;
    } else {
        return HTTPMethod::UNKNOWN;
    }
}

void parseHttpRequest(Buffer& buffer, HttpRequest& request)
{
    std::size_t current_pos = 0;
    std::size_t line_end_pos = 0;

    line_end_pos = buffer.find('\r', current_pos);
    if (line_end_pos == std::string::npos) {
        return;
    }

    std::string request_line_str = buffer.get_from_to(current_pos, line_end_pos - 1);

    std::istringstream iss(request_line_str);
    std::string method_str, uri_str, version_str;

    iss >> method_str >> uri_str >> version_str;

    if (method_str.empty() || uri_str.empty() || version_str.empty()) {
        return;
    }

    request.setMethod(getMethodFromStr(method_str));
    Uri u = uri_str;
    request.setUri(u);
    request.setVersion(version_str); 

    current_pos = line_end_pos + 2;

    while (true) {
        line_end_pos = buffer.find("\r\n", current_pos);
        if (line_end_pos == std::string::npos) {
            return;
        }

        if (line_end_pos == current_pos) {
            current_pos += 2;
            break;
        }

        std::string header_line = buffer.get_from_to(current_pos, line_end_pos - 1);

        std::size_t colon_pos = header_line.find(':');
        if (colon_pos == std::string::npos) {
            // Malformed header, just advance past it or handle error
        } else {
            std::string key = header_line.substr(0, colon_pos);
            std::string value = header_line.substr(colon_pos + 1);
            
            // Trim leading/trailing whitespace from value
            value.erase(0, value.find_first_not_of(" \t"));
            value.erase(value.find_last_not_of(" \t") + 1);

            request.setHeader(key, value);
        }

        current_pos = line_end_pos + 2;
    }

    std::string content_length_str = request.getHeader("Content-Length");
    if (!content_length_str.empty()) {
        try {
            std::size_t content_length = std::stoul(content_length_str);
            if (buffer.size() - current_pos < content_length) {
                // Not all body bytes received yet, append what's available
                request.setBody(buffer.data() + current_pos, buffer.size() - current_pos);
                current_pos = buffer.size(); // Consumed up to current buffer size
            } else {
                request.setBody(buffer.data() + current_pos, content_length);
                current_pos += content_length;
            }
        } catch (const std::exception& e) {
            // Handle error for invalid Content-Length
        }
    }

    buffer.advance_read_ptr(current_pos);
}

inline int hex_char_to_int(char c) {
    if (c >= '0' && c <= '9') {
        return c - '0';
    } else if (c >= 'a' && c <= 'f') {
        return 10 + (c - 'a');
    } else if (c >= 'A' && c <= 'F') {
        return 10 + (c - 'A');
    }
    throw std::runtime_error("Invalid hex character in URL encoding.");
}

std::string url_decode(const std::string& encoded_str) {
    std::string decoded_str;
    decoded_str.reserve(encoded_str.length()); 

    for (std::size_t i = 0; i < encoded_str.length(); ++i) {
        char c = encoded_str[i];

        if (c == '%') {
            if (i + 2 >= encoded_str.length() || !isxdigit(encoded_str[i+1]) || !isxdigit(encoded_str[i+2])) {
                throw std::runtime_error("Malformed URL encoding: invalid percent sequence.");
            }
            int high_nibble = hex_char_to_int(encoded_str[i+1]);
            int low_nibble = hex_char_to_int(encoded_str[i+2]);
            decoded_str += static_cast<char>((high_nibble << 4) | low_nibble);

            i += 2; 
        } else if (c == '+') {
            decoded_str += ' ';
        } else {
            decoded_str += c;
        }
    }

    return decoded_str;
}
