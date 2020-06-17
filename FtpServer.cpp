#include <iostream>
#include "net/ServerSocket.h"
#include "ServerControlChannel.h"

int main(int argc, char **argv) {
    int port = 21;
    std::string root = ".";

    if (argc >= 2)port = std::atoi(argv[1]);
    if (argc >= 3)root = argv[2];

    auto ss = new ServerSocket(port);

    std::cout << "FTP Server running at " << port << std::endl;
    std::cout << "Root dir is " << root << std::endl;

    srand (time(NULL));

    while (true) {
        std::cout << "Waiting for connection" << std::endl;
        auto socket = ss->accept();
        new ServerControlChannel(socket, root);
    }
    return 0;
}
