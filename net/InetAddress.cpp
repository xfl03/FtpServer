#include <arpa/inet.h>
#include "InetAddress.h"

InetAddress::InetAddress(in_addr_t address) {
    this->address = address;
}

InetAddress::InetAddress(std::string ip) {
    this->address=inet_addr(ip.c_str());
}