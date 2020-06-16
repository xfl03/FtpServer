#pragma once

#include <string>
#include "InetAddress.h"
#include "../io/FileInputStream.h"
#include "../io/FileOutputStream.h"

class Socket {
private:
    int fd;
    InetAddress *address;
public:
    Socket(int fd, InetAddress *address);

    Socket(std::string ip, int port);

    Socket(InetAddress *address, int port);

    FileInputStream *getInputStream();

    FileOutputStream *getOutputStream();

    void close();

    InetAddress *getInetAddress();
};