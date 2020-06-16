#include "FileHelper.h"
#include <filesystem>

namespace fs = std::filesystem;

FileHelper::FileHelper() {
    dir = "/";
}

bool FileHelper::changeDir(std::string path) {
    std::string tmp;
    if (path[0] == '/') {
        tmp = path;
    } else {
        tmp = dir + "/" + path;
    }
    auto status = fs::status("./" + tmp);
    if (fs::exists(status) && fs::is_directory(status)) {
        dir = tmp;
        return true;
    }
    return false;
}

void FileHelper::preWrite(std::string name) {
    os = new FileOutputStream("./" + dir + "/" + name);
}

void FileHelper::preRead(std::string name) {
    is = new FileInputStream("./" + dir + "/" + name);
}

void FileHelper::postWrite() {
    os->close();
    delete os;
    os = nullptr;
}

void FileHelper::postRead() {
    is->close();
    delete is;
    is = nullptr;
}