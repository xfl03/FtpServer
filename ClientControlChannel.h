#pragma once

#include "FileHelper.h"
#include "DataChannel.h"

#include "util/Scanner.h"
#include "io/PrintWriter.h"
#include "net/ServerSocket.h"

class ClientControlChannel {
private:
    FileHelper *file;
    DataChannel *data;
    Socket *socket;

    PrintWriter *wr;
    Scanner *sc;

    int port_port;
    ServerSocket* port_socket;

    bool pasv;
public:
    ClientControlChannel(bool pasv);

    ClientControlChannel(bool pasv,std::string ip,int port);

    void onCommand(std::string cmd, std::string arg);

    bool sendRequest(std::string cmd, std::string arg);

    std::pair<int, std::string>* readResponse();

    void printResponse(std::pair<int, std::string>* res);

    void close();

    void doLogin(std::string ip,std::string username);

    void connect(std::string ip,int port);

    void dataConnect();

    void displayHelp();
};