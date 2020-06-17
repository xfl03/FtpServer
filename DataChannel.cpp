#include "DataChannel.h"
#include "util/StringBuilder.h"
#include <thread>
#include <filesystem>
#include <iostream>

namespace DC {
    template<typename TP>
    std::time_t to_time_t(TP tp) {
        using namespace std::chrono;
        auto sctp = time_point_cast<system_clock::duration>(tp - TP::clock::now()
                                                            + system_clock::now());
        return system_clock::to_time_t(sctp);
    }

    std::string toTimeString(std::filesystem::file_time_type t) {
        std::time_t tt = to_time_t(t);
        std::tm *gmt = std::gmtime(&tt);
        std::stringstream buffer;
        buffer << std::put_time(gmt, "%b %d %Y ");
        return buffer.str();
    }
}

DataChannel::DataChannel(Socket *socket, FileHelper *file) {
    this->socket = socket;
    this->file = file;

    is = socket->getInputStream();
    os = socket->getOutputStream();
}

void DataChannel::recvFile(std::string name) {
    file->preWrite(name);

    char *buf = new char[333];
    while (true) {
        int len = is->read(buf, 0, 333);
        if (len <= 0) break;
        file->os->write(buf, 0, len);
    }

    file->postWrite();

    close();
}


void DataChannel::sendFile(std::string name) {
    file->preRead(name);

    char *buf = new char[333];
    while (true) {
        int len = file->is->read(buf, 0, 333);
        if (len <= 0) break;
        os->write(buf, 0, len);
    }

    file->postRead();

    close();
}

void DataChannel::sendList(bool name_only) {
    auto sb = new StringBuilder;
    for (const auto &entry : std::filesystem::directory_iterator(file->getRealPath())) {
        if (name_only) {
            sb->append(entry.path().filename().string());
        } else {
            if (entry.is_directory()) {
                sb->append("drwxrwxrwx 1 ftp ftp 0 ")
                        ->append(DC::toTimeString(entry.last_write_time()))
                        ->append(entry.path().filename().string());
            } else {
                sb->append("-rwxrwxrwx 1 ftp ftp ")
                        ->append((int) entry.file_size())->append(" ")
                        ->append(DC::toTimeString(entry.last_write_time()))
                        ->append(entry.path().filename().string());
            }
        }
        sb->append("\r\n");
    }
    std::string str = sb->toString();
    std::cout << "Send List:\r\n" << str;
    os->write(str.c_str(), 0, str.length());

    close();
}

void DataChannel::close() {
    socket->close();
}