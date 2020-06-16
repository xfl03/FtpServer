#include <iostream>
#include "net/ServerSocket.h"
#include "ControlChannel.h"

int main(int argc, char **argv) {
    int port = 21;
    if (argc >= 2)port = std::atoi(argv[1]);
    int pasv_port = port + 33;
    auto ss = new ServerSocket(port);
    std::cout<<"FTP Server running at "<<port<<std::endl;
    while (true) {
        std::cout<<"Waiting for connection"<<std::endl;
        auto socket = ss->accept();
        new ControlChannel(socket, pasv_port++);
    }
    return 0;
}
