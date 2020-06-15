#include "Socket.h"

#include <unistd.h>

auto close0 = close;

Socket::Socket(int fd) {
    this->fd = fd;
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