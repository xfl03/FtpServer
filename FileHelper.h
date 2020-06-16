#pragma once

#include "io/FileInputStream.h"
#include "io/FileOutputStream.h"

class FileHelper {
//private:
public:
    std::string dir;
    FileInputStream *is;
    FileOutputStream *os;

    FileHelper();

    bool changeDir(std::string path);

    void preWrite(std::string name);
    void preRead(std::string name);
    void postWrite();
    void postRead();
};
