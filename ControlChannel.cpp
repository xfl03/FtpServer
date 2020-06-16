#include "ControlChannel.h"
#include <thread>
#include <utility>
#include <iostream>
#include <filesystem>

#define USERNAME "test"
#define PASSWORD "test"

namespace fs = std::filesystem;

ControlChannel::ControlChannel(Socket *socket, int pasv_port) {
    this->socket = socket;
    file = new FileHelper;
    sc = new Scanner(socket->getInputStream());
    wr = new PrintWriter(socket->getOutputStream());
    this->pasv_port = pasv_port;

    std::thread t(&ControlChannel::run, this);
    t.detach();
}

void ControlChannel::run() {
    std::cout << "ControlChannel: run" << std::endl;
    sendResponse(220, "33's FTP Server");
    while (socket != nullptr) {
        std::string cmd = sc->next();
        if (sc->isLineFinished()) {
            onCommand(cmd, "");
        } else {
            onCommand(cmd, sc->readLine());
        }
    }
}

void ControlChannel::onCommand(std::string cmd, std::string arg) {
    std::cout << "onCommand: " << cmd << " " << arg << std::endl;
    std::transform(cmd.begin(), cmd.end(), cmd.begin(), ::tolower);
    if (cmd == "user") {
        user = std::move(arg);
        sendResponse(331, "Password required for " + user);
    } else if (cmd == "pass") {
        if (user == USERNAME && arg == PASSWORD) {
            sendResponse(230, "Logged on");
        } else {
            sendResponse(530, "Login or password incorrect!");
        }
    } else if (cmd == "pwd") {
        sendResponse(257, "\"" + file->dir + "\"");
    } else if (cmd == "type") {
        //TODO change type
        sendResponse(200, "Type changed.");
    } else if (cmd == "pasv") {
        pasvConnect();
    } else if (cmd == "port") {
        portConnect(arg);
    } else if (cmd == "retr") {
        sendResponse(150,
                     "Opening data channel for file download from server of \"" + file->dir + "/" + arg + "\"");
        data->sendFile(arg);
        sendResponse(226,
                     "Successfully transferred \"" + file->dir + "/" + arg + "\"");
    } else if (cmd == "stor") {
        sendResponse(150,
                     "Opening data channel for file upload to server of \"" + file->dir + "/" + arg + "\"");
        data->recvFile(arg);
        sendResponse(226,
                     "Successfully transferred \"" + file->dir + "/" + arg + "\"");
    } else if (cmd == "quit") {
        sendResponse(221, "Goodbye");
        socket->close();
        delete socket;
        socket = nullptr;
    } else if (cmd == "list") {
        sendResponse(150,
                     "Opening data channel for directory listing of \"" + file->dir + "\"");
        data->sendList();
        sendResponse(226,
                     "Successfully transferred \"" + file->dir + "\"");
    } else if (cmd == "cwd") {
        if (file->changeDir(arg)) {
            sendResponse(250, "CWD successful. \"" + file->dir + "\" is current directory.");
        } else {
            sendResponse(550, "CWD failed. \"/" + arg + "\": directory not found.");
        }
    } else if (cmd == "size") {
        auto status = fs::status("./" + arg);
        if (!fs::exists(status) || fs::is_directory(status)) {
            sendResponse(550, "File not found");
        } else {
            sendResponse(200, std::to_string(fs::file_size("./" + arg)));
        }
    } else {
        sendResponse(502, "?");
    }
}

void ControlChannel::sendResponse(int code, std::string msg) {
    std::cout << "sendResponse: " << code << " " << msg << std::endl;
    wr->println(std::to_string(code) + " " + msg);
}

void ControlChannel::pasvConnect() {
    auto ip = socket->getLocalAddress()->address;
    if (pasv_socket == nullptr) {
        pasv_socket = new ServerSocket(pasv_port);
    }
    unsigned int ips[4] = {ip >> 24, (ip >> 16) & 0xff, (ip >> 8) & 0xff, ip & 0xff};
    std::cout << "Passive Mode: " << ips[0] << "." << ips[1] << "." << ips[2] << "." << ips[3]
              << ":" << pasv_port << std::endl;
    sendResponse(227, "(" +
                      std::to_string(ips[0]) + "," +
                      std::to_string(ips[1]) + "," +
                      std::to_string(ips[2]) + "," +
                      std::to_string(ips[3]) + "," +
                      std::to_string(pasv_port >> 8) + "," +
                      std::to_string(pasv_port & 0xff) + ")");
    auto socket = pasv_socket->accept();
    data = new DataChannel(socket, file);
    std::cout << "Client connected in passive mode." << std::endl;
}

void ControlChannel::portConnect(std::string arg) {
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
