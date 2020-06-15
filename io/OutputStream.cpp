#include <unistd.h>
#include "OutputStream.h"

auto write0 = write;

OutputStream::OutputStream(int fd) {
    this->fd = fd;
}

void OutputStream::write(int b) {
    int *buf = new int[1];
    buf[0] = b;
    write0(fd, buf, 1);
}

void OutputStream::write(const char *b, int offset, int len) {
    write0(fd, b + offset, len);
}