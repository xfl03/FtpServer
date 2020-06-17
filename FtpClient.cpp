#include <iostream>

#include "util/Scanner.h"
#include "ClientControlChannel.h"

int main(int argc, char **argv) {
    bool pasv = false;
    std::string ip;
    int port = 21;

    if (argc >= 2) {
        if (argv[1][0] == '-') {
            if (argv[1][1] == 'p') {
                pasv = true;
            }
        } else {
            ip = argv[1];
        }
    }
    if (argc >= 3) {
        if (ip.empty()) {
            ip = argv[2];
        } else {
            port = std::atoi(argv[2]);
        }
    }
    if (argc >= 4) {
        port = std::atoi(argv[3]);
    }
    //std::cout << pasv << " " << ip << " " << port << std::endl;

    Scanner *sc = new Scanner(new FileInputStream(0));
    ClientControlChannel *client = ip.empty() ? new ClientControlChannel(pasv)
                                              : new ClientControlChannel(pasv, ip, port);
    while (true) {
        std::cout << "ftp> ";
        std::cout.flush();
        std::string cmd = sc->next();
        client->onCommand(cmd, sc->isLineFinished() ? "" : sc->readLine());
    }
}