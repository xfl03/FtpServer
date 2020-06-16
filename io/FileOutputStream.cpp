#include <unistd.h>
#include <fcntl.h>
#include "FileOutputStream.h"

auto write0 = write;

FileOutputStream::FileOutputStream(int fd) {
    this->fd = fd;
}

FileOutputStream::FileOutputStream(std::string name) {
    fd = open(name.c_str(), O_WRONLY | O_CREAT, S_IRWXU);
}

void FileOutputStream::write(int b) {
    int *buf = new int[1];
    buf[0] = b;
    write0(fd, buf, 1);
}

void FileOutputStream::write(const char *b, int offset, int len) {
    write0(fd, b + offset, len);
}