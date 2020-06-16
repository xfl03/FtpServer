#pragma once
#include <string>
#include <netinet/in.h>
class InetAddress{
public:
    in_addr_t address;
    InetAddress(in_addr_t address);
    InetAddress(std::string ip);
};