#include "FileInputStream.h"
#include <fcntl.h>
#include <unistd.h>

auto read0 = read;

FileInputStream::FileInputStream(int fd) {
    this->fd = fd;
}

FileInputStream::FileInputStream(std::string name) {
    fd = open(name.c_str(), O_RDONLY);
}

int FileInputStream::read() {
    int *buffer = new int[1];
    read0(fd, buffer, 1);
    return buffer[0];
}

int FileInputStream::read(char *b, int offset, int len) {
    return (int) read0(fd, b + offset, len);
}