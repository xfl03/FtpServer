#include "FileHelper.h"
#include <filesystem>

namespace fs = std::filesystem;

FileHelper::FileHelper(std::string root) {
    dir = "/";
    this->root = root;
}

bool FileHelper::changeDir(std::string path) {
    std::string tmp;
    if (path[0] == '/') {
        tmp = path;
    } else {
        tmp = dir + "/" + path;
    }
    auto status = fs::status(root + tmp);
    if (fs::exists(status) && fs::is_directory(status)) {
        dir = tmp;
        return true;
    }
    return false;
}

void FileHelper::preWrite(std::string name) {
    os = new FileOutputStream(getRealPath(name));
}

void FileHelper::preRead(std::string name) {
    is = new FileInputStream(getRealPath(name));
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

std::string FileHelper::getRealPath() {
    return root + dir;
}

std::string FileHelper::getRealPath(std::string name) {
    if (name[0] == '/') {
        return root + name;
    } else {
        return root + dir + "/" + name;
    }
}

std::string FileHelper::getDisplayPath() {
    return dir;
}

std::string FileHelper::getDisplayPath(std::string name) {
    if (name[0] == '/') {
        return name;
    } else {
        return dir + "/" + name;
    }
}