#include "InputStream.h"

#include <unistd.h>

auto read0 = read;

InputStream::InputStream(int fd) {
    this->fd = fd;
}

int InputStream::read() {
    int *buffer = new int[1];
    read0(fd, buffer, 1);
    return buffer[0];
}