#include "Socket.h"

#include <stdexcept>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

auto close0 = close;

Socket::Socket(int fd) {
    this->fd = fd;
}

Socket::Socket(std::string ip, int port) {
    if (port < 0 || port > 65535) throw std::invalid_argument("Illegal port.");
    auto addr = new sockaddr_in;
    addr->sin_family = AF_INET;
    addr->sin_addr.s_addr = inet_addr(ip.c_str());
    addr->sin_port = htons(port);

    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) throw std::runtime_error("Failed to open socket.");

    if (connect(fd, (sockaddr *) addr, sizeof(sockaddr_in)) < 0) {
        throw std::runtime_error("Failed to connect socket.");
    }
}

InputStream *Socket::getInputStream() {
    return new InputStream(fd);
}

OutputStream *Socket::getOutputStream() {
    return new OutputStream(fd);
}

void Socket::close() {
    close0(fd);
}