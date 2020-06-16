#include "Socket.h"

#include <stdexcept>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

auto close0 = close;

Socket::Socket(int fd, InetAddress *address) {
    this->fd = fd;
    this->address = address;
}

Socket::Socket(std::string ip, int port)
        : Socket(new InetAddress(ip), port) {
}

Socket::Socket(InetAddress *address, int port) {
    if (port < 0 || port > 65535) throw std::invalid_argument("Illegal port.");

    auto addr = new sockaddr_in;
    addr->sin_family = AF_INET;
    addr->sin_addr.s_addr = address->address;
    addr->sin_port = htons(port);

    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) throw std::runtime_error("Failed to open socket.");

    if (connect(fd, (sockaddr *) addr, sizeof(sockaddr_in)) < 0) {
        throw std::runtime_error("Failed to connect socket.");
    }

    this->address = address;
}

FileInputStream *Socket::getInputStream() {
    return new FileInputStream(fd);
}

FileOutputStream *Socket::getOutputStream() {
    return new FileOutputStream(fd);
}

void Socket::close() {
    close0(fd);
}

InetAddress *Socket::getInetAddress() {
    //getpeername
    return address;
}

InetAddress *Socket::getLocalAddress() {
    auto addr = new sockaddr_in;
    socklen_t length = sizeof(sockaddr_in);
    getsockname(fd, (sockaddr *) addr, &length);
    return new InetAddress(addr->sin_addr.s_addr);
}