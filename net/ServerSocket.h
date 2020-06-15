#pragma once

#include "Socket.h"

class ServerSocket {
private:
    int fd;
public:
    ServerSocket(int port);

    Socket *accept();

    void close();
};
