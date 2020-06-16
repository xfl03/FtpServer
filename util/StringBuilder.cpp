#include "StringBuilder.h"

StringBuilder *StringBuilder::append(char c) {
    ss << c;
    return this;
}
StringBuilder *StringBuilder::append(std::string c) {
    ss << c;
    return this;
}
StringBuilder *StringBuilder::append(int c) {
    ss << c;
    return this;
}

std::string StringBuilder::toString() {
    return ss.str();
}