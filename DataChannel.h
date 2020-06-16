#pragma once

#include "FileHelper.h"
#include "net/Socket.h"

class DataChannel {
private:
    FileHelper *file;
    Socket *socket;
    FileInputStream* is;
    FileOutputStream* os;
public:
    DataChannel(Socket *socket, FileHelper *file);
    void recvFile(std::string name);
    void sendFile(std::string name);
    void sendList();
    void close();
};
