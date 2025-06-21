#ifndef UTILS_H
#define UTILS_H
#include <string>
#include "Types.hpp"
#include "HttpRequest.hpp"

constexpr char NULL_TERMINATOR = '\0';

u16 mHTONS(u16 port);
const std::string getMethodStr(HTTPMethod method);
void parseHttpRequest(Buffer& buffer, HttpRequest& request);
//u16 mINET_PTON

#endif // !UTILS

