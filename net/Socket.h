#pragma once

#include <string>
#include "../io/InputStream.h"
#include "../io/OutputStream.h"

class Socket {
private:
    int fd;
public:
    Socket(int fd);

    Socket(std::string ip, int port);

    InputStream *getInputStream();

    OutputStream *getOutputStream();

    void close();
};