#pragma once
#include <string>
class FileInputStream {
private:
    int fd;
public:
    FileInputStream(int fd);
    FileInputStream(std::string name);
    int read();
    int read(char* b,int offset, int len);
    void close();
};
