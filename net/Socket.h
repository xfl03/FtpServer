#pragma once

#include "../io/InputStream.h"
#include "../io/OutputStream.h"

class Socket{
private:
    int fd;
public:
    Socket(int fd);
    InputStream* getInputStream();
    OutputStream* getOutputStream();
    void close();
};