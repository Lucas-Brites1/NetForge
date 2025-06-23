#ifndef UTILS_H
#define UTILS_H
#include <string>
#include "Types.hpp"
#include "HttpRequest.hpp"

constexpr char NULL_TERMINATOR = '\0';

u16 mHTONS(u16 port);
const std::string getMethodStr(HTTPMethod method);
void parseHttpRequest(Buffer& buffer, HttpRequest& request);
std::string url_decode(const std::string& encoded_str);
std::string url_encode(const std::string& str);
std::string url_encode(const Uri& uri);
std::string escape_string(std::string original_str);

//u16 mINET_PTON

#endif // !UTILS

