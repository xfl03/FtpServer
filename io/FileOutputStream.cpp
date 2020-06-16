#include "FileOutputStream.h"
#include <stdexcept>
#include <unistd.h>
#include <fcntl.h>

namespace FOS {
    auto write0 = write;
    auto close0 = close;
}

FileOutputStream::FileOutputStream(int fd) {
    this->fd = fd;
}

FileOutputStream::FileOutputStream(std::string name) {
    fd = open(name.c_str(), O_WRONLY | O_CREAT, S_IRWXU);
    if (fd < 0) throw std::runtime_error("Failed to open file.");
}

void FileOutputStream::write(int b) {
    int *buf = new int[1];
    buf[0] = b;
    FOS::write0(fd, buf, 1);
}

void FileOutputStream::write(const char *b, int offset, int len) {
    FOS::write0(fd, b + offset, len);
}

void FileOutputStream::close() {
    FOS::close0(fd);
}