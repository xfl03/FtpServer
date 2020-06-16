#pragma once
#include <sstream>
#include <string>
class StringBuilder {
private:
    std::stringstream ss;
public:
    StringBuilder* append(char c);
    StringBuilder* append(std::string c);
    StringBuilder* append(int i);
    std::string toString();
};
