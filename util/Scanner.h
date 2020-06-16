#pragma once
#include <string>
#include "../io/FileInputStream.h"
class Scanner {
private:
    FileInputStream* is;
    char* buffer;
    int size;
    int pos;
    char nextChar();
    std::string nextStr(bool blank);
    bool lineFinished;
public:
    Scanner(FileInputStream* is);
    std::string next();
    int nextInt();
    bool isLineFinished();// Extended method
    std::string readLine();// Extended method
};
