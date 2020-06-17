#include "ClientControlChannel.h"

#include <iostream>
#include <sstream>
#include <iomanip>

ClientControlChannel::ClientControlChannel(bool pasv) {
    file = new FileHelper(".");
    this->pasv = pasv;
}

ClientControlChannel::ClientControlChannel(bool pasv, std::string ip, int port)
        : ClientControlChannel(pasv) {
    connect(ip, port);
}

void ClientControlChannel::onCommand(std::string cmd, std::string arg) {
    //std::cout << cmd << " " << arg << std::endl;
    std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::tolower);

    if (cmd == "open") {
        std::string ip = arg;
        int port = 21;
        if (arg.find(' ') > 0) {
            std::stringstream ss;
            ss << arg;
            ss >> ip >> port;
        }
        connect(ip, port);
    } else if (cmd == "user") {
        doLogin("", arg);
    } else if (cmd == "bye" || cmd == "quit" || cmd == "exit" || cmd == "!") {
        close();
        exit(0);
    } else if (cmd == "close" || cmd == "disconnect") {
        close();
    } else if (cmd == "dir" || cmd == "ls") {
        dataConnect();
        sendRequest("LIST", "");
        auto res = readResponse();
        printResponse(res);
        if (res->first != 150) return;
        std::cout << data->recvList();
        printResponse(readResponse());
    } else if (cmd == "get" || cmd == "recv") {
        dataConnect();
        std::string from = arg;
        std::string to = arg;
        if (arg.find(' ') > 0) {
            std::stringstream ss;
            ss << arg;
            ss >> from >> to;
        }
        sendRequest("RETR", from);
        auto res = readResponse();
        printResponse(res);
        if (res->first != 150) return;
        data->recvFile(to);
        printResponse(readResponse());
    } else if (cmd == "cd") {
        sendRequest("CWD", arg);
        printResponse(readResponse());
    } else if (cmd == "delete") {
        sendRequest("DELE", arg);
        printResponse(readResponse());
    } else if (cmd == "rmdir") {
        sendRequest("RMD", arg);
        printResponse(readResponse());
    } else if (cmd == "mkdir") {
        sendRequest("MKD", arg);
        printResponse(readResponse());
    } else if (cmd == "size") {
        sendRequest("SIZE", arg);
        printResponse(readResponse());
    } else if (cmd == "pwd") {
        sendRequest("PWD", arg);
        printResponse(readResponse());
    } else if (cmd == "system") {
        sendRequest("SYST", arg);
        printResponse(readResponse());
    } else if (cmd == "rename") {
        std::string from;
        std::string to;
        if (arg.find(' ') > 0) {
            std::stringstream ss;
            ss << arg;
            ss >> from >> to;
        }
        sendRequest("RNFR", from);
        auto res = readResponse();
        printResponse(res);
        if (res->first != 350) return;
        sendRequest("RNTO", to);
        printResponse(readResponse());
    } else if (cmd == "passive") {
        pasv = !pasv;
        std::cout << "Passive mode " << (pasv ? "on." : "off.") << std::endl;
    } else if (cmd == "put" || cmd == "send") {
        dataConnect();
        std::string from = arg;
        std::string to = arg;
        if (arg.find(' ') > 0) {
            std::stringstream ss;
            ss << arg;
            ss >> from >> to;
        }
        sendRequest("STOR", to);
        auto res = readResponse();
        printResponse(res);
        if (res->first != 150) return;
        data->sendFile(from);
        printResponse(readResponse());
    } else if (cmd == "help" || cmd == "?") {
        displayHelp();
    } else {
        std::cout << "?Invalid command" << std::endl;
    }
}

void ClientControlChannel::displayHelp() {
    std::string cmds[] = {
            "!", "bye", "cd", "close", "delete",
            "dir", "disconnect", "exit", "get", "help",
            "ls", "mkdir", "open", "passive", "put",
            "pwd", "quit", "recv", "rename", "rmdir",
            "send", "size", "system", "user", "?"
    };
    int size = 25;
    for (int i = 0; i < size / 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            int index = j * 5 + i;
            if (index < size) {
                std::cout << std::setw(14) << std::setfill(' ') << std::left << cmds[index];
                //if(j!=4) std::cout << "\t\t";
            }
        }
        std::cout << std::endl;
    }
}

bool ClientControlChannel::sendRequest(std::string cmd, std::string arg) {
    if (socket == nullptr) {
        std::cout << "Not connected." << std::endl;
        return false;
    }

    wr->println(cmd + " " + arg);
    return true;
}

std::pair<int, std::string> *ClientControlChannel::readResponse() {
    if (socket == nullptr) throw std::runtime_error("Not connected.");

    int code = sc->nextInt();
    return new std::pair<int, std::string>(code, sc->isLineFinished() ? "" : sc->readLine());
}

void ClientControlChannel::printResponse(std::pair<int, std::string> *res) {
    std::cout << res->first << " " << res->second << std::endl;
}

void ClientControlChannel::connect(std::string ip, int port) {
    try {
        socket = new Socket(ip, port);

        std::cout << "Connected to " << ip << "." << std::endl;

        wr = new PrintWriter(socket->getOutputStream());
        sc = new Scanner(socket->getInputStream());

        auto res = readResponse();
        printResponse(res);
        if (res->first != 220) {
            std::cout << "?" << std::endl;
            return;
        }

        doLogin(ip, "");

    } catch (std::runtime_error &e) {
        std::cout << "ftp: connect: Connection refused" << std::endl;
    }
}

void ClientControlChannel::doLogin(std::string ip, std::string username) {
    std::string tmp = username;
    if (tmp.empty()) {
        std::cout << "Name (" << ip << "): ";
        std::cin >> tmp;
    }
    sendRequest("USER", tmp);

    auto res = readResponse();
    printResponse(res);
    if (res->first == 331) {
        std::cout << "Password: ";
        std::cin >> tmp;
        sendRequest("PASS", tmp);

        res = readResponse();
        printResponse(res);
        if (res->first == 530) {
            std::cout << "Login failed." << std::endl;
            //close();
            return;
        }
    }

    sendRequest("SYST", "");
    res = readResponse();
    std::cout << "Remote system type is " << res->second << "." << std::endl;
}

void ClientControlChannel::close() {
    if (socket != nullptr) {
        sendRequest("QUIT", "");
        printResponse(readResponse());
        socket->close();
        delete socket;
        socket = nullptr;
    }
    if (port_socket != nullptr) {
        port_socket->close();
        delete port_socket;
        port_socket = nullptr;
    }
}

void ClientControlChannel::dataConnect() {
    if (data != nullptr) {
        data->close();
        delete data;
    }

    if (pasv) {
        sendRequest("PASV", "");
        auto res = readResponse();
        printResponse(res);
        if (res->first != 227) {
            return;
        }

        int t = 0;
        std::string ip;
        int port0 = 0;
        int port1 = 0;
        for (char c:res->second) {
            if (c == '(' || c == ')' || c == ' ') continue;
            if (c == ',') {
                if (t < 3) ip += ".";
                t++;
            } else {
                if (t < 4) {
                    ip += c;
                } else if (t == 4) {
                    port0 *= 10;
                    port0 += c - '0';
                } else {
                    port1 *= 10;
                    port1 += c - '0';
                }
            }
        }
        int port = port0 * 256 + port1;
        //std::cout << "Passive Mode: " << ip << ":" << port << std::endl;
        data = new DataChannel(new Socket(ip, port), file);
    } else {
        auto ip = socket->getLocalAddress()->address;

        if (port_socket == nullptr) {
            port_port = rand() % 3975 + 1025;
            //Port usage check
            for (int i = 0; i < 3; ++i) {
                try {
                    //Try to bind port
                    //std::cout << "Try to bind " << port_port << std::endl;
                    port_socket = new ServerSocket(port_port);
                    break;
                } catch (std::runtime_error &e) {
                    //When port has been used
                    port_port = rand() % 3975 + 1025;
                }
            }
        }
        unsigned int ips[4] = {ip >> 24, (ip >> 16) & 0xff, (ip >> 8) & 0xff, ip & 0xff};
        //std::cout << "Active Mode: " << ips[3] << "." << ips[2] << "." << ips[1] << "." << ips[0]
        //          << ":" << port_port << std::endl;
        sendRequest("PORT",
                    std::to_string(ips[3]) + "," +
                    std::to_string(ips[2]) + "," +
                    std::to_string(ips[1]) + "," +
                    std::to_string(ips[0]) + "," +
                    std::to_string(port_port >> 8) + "," +
                    std::to_string(port_port & 0xff));
        auto socket = port_socket->accept();
        data = new DataChannel(socket, file);
        //std::cout << "Server connected in active mode." << std::endl;


        auto res = readResponse();
        printResponse(res);
        if (res->first != 200) {
            return;
        }
    }
}