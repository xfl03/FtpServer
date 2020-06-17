#include "ServerSocket.h"
#include <stdexcept>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

auto accept0 = accept;
auto close1 = close;

ServerSocket::ServerSocket(int port) {
    if (port < 0 || port > 65535) throw std::invalid_argument("Illegal port.");
    auto addr = new sockaddr_in;
    addr->sin_family = AF_INET;
    addr->sin_addr.s_addr = INADDR_ANY;
    addr->sin_port = htons(port);

    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) throw std::runtime_error("Failed to open socket.");

    if (bind(fd, (sockaddr *) addr, sizeof(sockaddr_in)) < 0) throw std::runtime_error("Failed to bind socket.");

    if (listen(fd, 33)) throw std::runtime_error("Failed to listen socket.");
}

Socket *ServerSocket::accept() {
    auto addr = new sockaddr_in;
    socklen_t length = sizeof(sockaddr_in);
    int new_fd = accept0(fd, (sockaddr *) addr, &length);
    return new Socket(new_fd, new InetAddress(addr->sin_addr.s_addr));
}

void ServerSocket::close() {
    shutdown(fd, SHUT_RDWR);
    close1(fd);
}