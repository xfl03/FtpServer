#pragma once

#include <string>
#include "FileOutputStream.h"

class PrintWriter {
private:
    FileOutputStream* os;
public:
    PrintWriter(FileOutputStream* os);
    void write(std::string str);
    void println(std::string str);
};
