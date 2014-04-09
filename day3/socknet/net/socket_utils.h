#pragma once

#include <vector>
#include <string>
#include <cstdint>

namespace net
{
std::string resolveHostname( const std::string& hostname);
uint16_t resolveService( const std::string& service, const std::string& protocol = "tcp");
}
