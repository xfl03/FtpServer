#include "ServerControlChannel.h"
#include <thread>
#include <utility>
#include <iostream>
#include <filesystem>

#define USERNAME "test"
#define PASSWORD "test"

namespace fs = std::filesystem;

ServerControlChannel::ServerControlChannel(Socket *socket, std::string root) {
    this->socket = socket;
    file = new FileHelper(root);
    sc = new Scanner(socket->getInputStream());
    wr = new PrintWriter(socket->getOutputStream());

    std::thread t(&ServerControlChannel::run, this);
    t.detach();
}

void ServerControlChannel::run() {
    std::cout << "\nServerControlChannel: run" << std::endl;
    sendResponse(220, "33's FTP Server");
    while (socket != nullptr) {
        std::string cmd = sc->next();
        if (cmd.empty()) {
            std::cout << "Socket closed." << std::endl;
            close();
        } else {
            onCommand(cmd, sc->isLineFinished() ? "" : sc->readLine());
        }
        //std::cout << "ServerControlChannel: onCommand finish" << std::endl;
    }
    std::cout << "ServerControlChannel: stop" << std::endl;
}

void ServerControlChannel::onCommand(std::string cmd, std::string arg) {
    std::cout << "onCommand: " << cmd << " " << arg << std::endl;
    std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::tolower);

    //Logon
    if (cmd == "user") {
        tmp = std::move(arg);
        sendResponse(331, "Password required for " + tmp);
        return;
    }
    if (cmd == "pass") {
        if (tmp == USERNAME && arg == PASSWORD) {
            sendResponse(230, "Logged on");
            logon = true;
        } else {
            sendResponse(530, "Login or password incorrect!");
        }
        return;
    }
    if (cmd == "quit") {
        sendResponse(221, "Goodbye");
        close();
        return;
    }

    //Logon check
    if (!logon) {
        sendResponse(530, "Please log in with USER and PASS first.");
        return;
    }
    if (cmd == "pwd") {
        sendResponse(257, "\"" + file->getDisplayPath() + "\"");
    } else if (cmd == "type") {
        //TODO change type
        sendResponse(200, "Type set to " + arg);
    } else if (cmd == "pasv") {
        pasvConnect();
    } else if (cmd == "port") {
        portConnect(arg);
    } else if (cmd == "retr") {
        auto status = fs::status(file->getRealPath(arg));
        if (!fs::exists(status) || fs::is_directory(status)) {
            sendResponse(550, "File not found");
        } else {
            sendResponse(150,
                         "Opening data channel for file download from server of \"" + file->getDisplayPath(arg) + "\"");
            data->sendFile(arg);
            sendResponse(226,
                         "Successfully transferred \"" + file->getDisplayPath(arg) + "\"");
        }
    } else if (cmd == "stor") {
        sendResponse(150,
                     "Opening data channel for file upload to server of \"" + file->getDisplayPath(arg) + "\"");
        data->recvFile(arg);
        sendResponse(226,
                     "Successfully transferred \"" + file->getDisplayPath(arg) + "\"");
    } else if (cmd == "list" || cmd == "nlst") {
        sendResponse(150,
                     "Opening data channel for directory listing of \"" + file->getDisplayPath() + "\"");
        data->sendList(cmd == "nlst");
        sendResponse(226,
                     "Successfully transferred \"" + file->getDisplayPath() + "\"");
    } else if (cmd == "cwd") {
        if (file->changeDir(arg)) {
            sendResponse(250, "CWD successful. \"" + file->getDisplayPath() + "\" is current directory.");
        } else {
            sendResponse(550, "CWD failed. \"" + arg + "\": directory not found.");
        }
    } else if (cmd == "size") {
        auto status = fs::status(file->getRealPath(arg));
        if (!fs::exists(status) || fs::is_directory(status)) {
            sendResponse(550, "File not found");
        } else {
            sendResponse(213, std::to_string(fs::file_size(file->getRealPath(arg))));
        }
    } else if (cmd == "opts") {
        sendResponse(202, "!");
    } else if (cmd == "dele") {
        auto status = fs::status(file->getRealPath(arg));
        if (!fs::exists(status) || fs::is_directory(status)) {
            sendResponse(550, "File not found");
        } else {
            fs::remove(file->getRealPath(arg));
            sendResponse(250, "File deleted successfully");
        }
    } else if (cmd == "rmd") {
        auto status = fs::status(file->getRealPath(arg));
        if (!fs::exists(status) || !fs::is_directory(status)) {
            sendResponse(550, "Directory not found");
        } else {
            fs::remove_all(file->getRealPath(arg));
            sendResponse(250, "Directory deleted successfully");
        }
    } else if (cmd == "noop") {
        sendResponse(200, "OK");
    } else if (cmd == "syst") {
        sendResponse(215, "UNIX");
    } else if (cmd == "rnfr") {
        auto status = fs::status(file->getRealPath(arg));
        if (!fs::exists(status)) {
            sendResponse(550, "File not found");
        } else {
            tmp = arg;
            sendResponse(350, "File exists, ready for destination name.");
        }
    } else if (cmd == "rnto") {
        fs::rename(file->getRealPath(tmp), file->getRealPath(arg));
        sendResponse(250, "file renamed successfully");
    } else if (cmd == "mkd") {
        fs::create_directories(file->getRealPath(arg));
        sendResponse(257, "\"" + file->getDisplayPath(arg) + "\" created successfully");
    } else {
        sendResponse(502, "?");
    }
}

void ServerControlChannel::sendResponse(int code, std::string msg) {
    std::cout << "sendResponse: " << code << " " << msg << std::endl;
    wr->println(std::to_string(code) + " " + msg);
}

void ServerControlChannel::pasvConnect() {
    if (data != nullptr) {
        data->close();
        delete data;
    }

    auto ip = socket->getLocalAddress()->address;

    if (pasv_socket == nullptr) {
        pasv_port = rand() % 3975 + 1025;
        //Port usage check
        for (int i = 0; i < 3; ++i) {
            try {
                //Try to bind port
                std::cout << "Try to bind " << pasv_port << std::endl;
                pasv_socket = new ServerSocket(pasv_port);
                break;
            } catch (std::runtime_error &e) {
                //When port has been used
                pasv_port = rand() % 3975 + 1025;
            }
        }
    }
    unsigned int ips[4] = {ip >> 24, (ip >> 16) & 0xff, (ip >> 8) & 0xff, ip & 0xff};
    std::cout << "Passive Mode: " << ips[3] << "." << ips[2] << "." << ips[1] << "." << ips[0]
              << ":" << pasv_port << std::endl;
    sendResponse(227, "(" +
                      std::to_string(ips[3]) + "," +
                      std::to_string(ips[2]) + "," +
                      std::to_string(ips[1]) + "," +
                      std::to_string(ips[0]) + "," +
                      std::to_string(pasv_port >> 8) + "," +
                      std::to_string(pasv_port & 0xff) + ")");
    auto socket = pasv_socket->accept();
    data = new DataChannel(socket, file);
    std::cout << "Client connected in passive mode." << std::endl;
}

void ServerControlChannel::portConnect(std::string arg) {
    if (data != nullptr) {
        data->close();
        delete data;
    }

    int t = 0;
    std::string ip;
    int port0 = 0;
    int port1 = 0;
    for (char c:arg) {
        if (c == ' ') continue;
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
    std::cout << "Active Mode: " << ip << ":" << port << std::endl;
    data = new DataChannel(new Socket(ip, port), file);
    sendResponse(200, "Port command successful");
}

void ServerControlChannel::close() {
    if (socket != nullptr) {
        socket->close();
        delete socket;
        socket = nullptr;
    }
    closePasv();
}

void ServerControlChannel::closePasv() {
    if (pasv_socket != nullptr) {
        pasv_socket->close();
        delete pasv_socket;
        pasv_socket = nullptr;
    }
}
