#pragma once
#include <string>
#include "../io/InputStream.h"
class Scanner {
private:
    InputStream* is;
public:
    Scanner(InputStream* is);
    std::string next();
    int nextInt();
};
