#include "DataChannel.h"
#include "util/StringBuilder.h"
#include "util/Scanner.h"
#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;
namespace DC {
    template<typename TP>
    std::time_t to_time_t(TP tp) {
        using namespace std::chrono;
        auto sctp = time_point_cast<system_clock::duration>(tp - TP::clock::now()
                                                            + system_clock::now());
        return system_clock::to_time_t(sctp);
    }

    std::string toTimeString(std::filesystem::file_time_type t) {
        auto tt0 = time(NULL);
        std::tm *gmt0 = std::gmtime(&tt0);

        std::time_t tt = to_time_t(t);
        std::tm *gmt = std::gmtime(&tt);

        std::stringstream buffer;
        if (gmt0->tm_year == gmt->tm_year) {
            buffer << std::put_time(gmt, "%b %d %H:%M ");
        } else {
            buffer << std::put_time(gmt, "%b %d %Y ");
        }
        return buffer.str();
    }

    std::string toPermString(fs::perms p) {
        std::stringstream ss;
        ss << ((p & fs::perms::owner_read) != fs::perms::none ? "r" : "-")
           << ((p & fs::perms::owner_write) != fs::perms::none ? "w" : "-")
           << ((p & fs::perms::owner_exec) != fs::perms::none ? "x" : "-")
           << ((p & fs::perms::group_read) != fs::perms::none ? "r" : "-")
           << ((p & fs::perms::group_write) != fs::perms::none ? "w" : "-")
           << ((p & fs::perms::group_exec) != fs::perms::none ? "x" : "-")
           << ((p & fs::perms::others_read) != fs::perms::none ? "r" : "-")
           << ((p & fs::perms::others_write) != fs::perms::none ? "w" : "-")
           << ((p & fs::perms::others_exec) != fs::perms::none ? "x" : "-");
        return ss.str();
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
        //std::cout << len << std::endl;
        if (len <= 0) break;
        file->os->write(buf, 0, len);
    }
    delete[] buf;

    file->postWrite();

    close();
}


void DataChannel::sendFile(std::string name) {
    file->preRead(name);

    char *buf = new char[333];

    while (true) {
        int len = file->is->read(buf, 0, 333);
        //std::cout << len << std::endl;
        if (len <= 0) break;
        os->write(buf, 0, len);
    }
    delete[] buf;

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
                sb->append("d")->append(DC::toPermString(entry.status().permissions()))
                        ->append(" 1 ftp ftp              0 ")
                        ->append(DC::toTimeString(entry.last_write_time()))
                        ->append(entry.path().filename().string());
            } else {
                std::stringstream ss;
                ss << std::setw(14) << std::setfill(' ') << std::right << entry.file_size();
                sb->append("-")->append(DC::toPermString(entry.status().permissions()))
                        ->append(" 1 ftp ftp ")
                        ->append(ss.str())->append(" ")
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

std::string DataChannel::recvList() {
    auto sb = new StringBuilder;
    Scanner *sc = new Scanner(is);
    while (true) {
        std::string tmp = sc->readLine();
        if (tmp.empty()) break;
        sb->append(tmp)->append("\n");
    }
    return sb->toString();
    close();
}

void DataChannel::close() {
    socket->close();
}