#include "StringBuilder.h"

StringBuilder *StringBuilder::append(char c) {
    ss << c;
    return this;
}

std::string StringBuilder::toString() {
    return ss.str();
}