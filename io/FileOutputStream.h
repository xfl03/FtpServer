#pragma once
#include <string>
class FileOutputStream {
private:
    int fd;
public:
    FileOutputStream(int fd);
    FileOutputStream(std::string name);
    void write(int b);
    void write(const char* b,int offset, int len);
};
