#pragma once

#include <string>
#include "../io/OutputStream.h"

class PrintWriter {
private:
    OutputStream* os;
public:
    PrintWriter(OutputStream* os);
    void write(std::string str);
};
