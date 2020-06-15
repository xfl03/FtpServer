#pragma once
#include <sstream>
#include <string>
class StringBuilder {
private:
    std::stringstream ss;
public:
    StringBuilder* append(char c);
    std::string toString();
};
