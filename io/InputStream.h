#pragma once
class InputStream {
private:
    int fd;
public:
    InputStream(int fd);
    int read();
};
