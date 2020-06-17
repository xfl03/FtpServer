#pragma once

#include "net/Socket.h"
#include "io/PrintWriter.h"
#include "util/Scanner.h"
#include "DataChannel.h"
#include "FileHelper.h"
#include "net/ServerSocket.h"

class ControlChannel {
private:
    Socket *socket;
    DataChannel *data;
    FileHelper *file;

    PrintWriter *wr;
    Scanner *sc;

    int pasv_port;
    ServerSocket* pasv_socket;

    std::string user;
public:
    void run();

    ControlChannel(Socket *socket, int pasv_port, std::string root);

    void onCommand(std::string cmd, std::string arg);

    void sendResponse(int code, std::string msg);

    void pasvConnect();

    void portConnect(std::string arg);

    void close();
};