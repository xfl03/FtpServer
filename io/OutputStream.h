#pragma once
class OutputStream {
private:
    int fd;
public:
    OutputStream(int fd);
    void write(int b);
    void write(const char* b,int offset, int len);
};
