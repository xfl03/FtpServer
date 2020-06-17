#pragma once

#include "net/Socket.h"
#include "io/PrintWriter.h"
#include "util/Scanner.h"
#include "DataChannel.h"
#include "FileHelper.h"
#include "net/ServerSocket.h"

class ServerControlChannel {
private:
    Socket *socket;
    DataChannel *data;
    FileHelper *file;

    PrintWriter *wr;
    Scanner *sc;

    int pasv_port;
    ServerSocket* pasv_socket;

    std::string tmp;
    bool logon;
public:
    void run();

    ServerControlChannel(Socket *socket, std::string root);

    void onCommand(std::string cmd, std::string arg);

    void sendResponse(int code, std::string msg);

    void pasvConnect();

    void portConnect(std::string arg);

    void close();

    void closePasv();
};