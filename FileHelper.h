#pragma once

#include "io/FileInputStream.h"
#include "io/FileOutputStream.h"

class FileHelper {
//private:
public:
    std::string dir;
    std::string root;
    FileInputStream *is;
    FileOutputStream *os;

    FileHelper(std::string root);

    bool changeDir(std::string path);

    void preWrite(std::string name);

    void preRead(std::string name);

    void postWrite();

    void postRead();

    std::string getRealPath();

    std::string getDisplayPath();

    std::string getRealPath(std::string name);

    std::string getDisplayPath(std::string name);
};
