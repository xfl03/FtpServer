#include "FileInputStream.h"
#include <stdexcept>
#include <fcntl.h>
#include <unistd.h>

namespace FIS {
    auto read0 = read;
    auto close0 = close;
}

FileInputStream::FileInputStream(int fd) {
    this->fd = fd;
}

FileInputStream::FileInputStream(std::string name) {
    fd = open(name.c_str(), O_RDONLY);
    if (fd < 0) throw std::runtime_error("Failed to open file.");
}

int FileInputStream::read() {
    int *buffer = new int[1];
    FIS::read0(fd, buffer, 1);
    return buffer[0];
}

int FileInputStream::read(char *b, int offset, int len) {
    return (int) FIS::read0(fd, b + offset, len);
}

void FileInputStream::close() {
    FIS::close0(fd);
}