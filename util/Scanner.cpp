#include "Scanner.h"

#include "StringBuilder.h"

Scanner::Scanner(FileInputStream *is) {
    this->is = is;
    buffer = new char[233];
    size = 0;
    pos = 0;
}

char Scanner::nextChar() {
    if (pos >= size) {
        size = is->read(buffer, 0, 233);
        pos = 0;
    }
    return buffer[pos++];
}

std::string Scanner::nextStr(bool blank) {

    StringBuilder sb;
    char c = nextChar();
    if (c == '\n') c = nextChar(); // Clear first LF
    while (true) {
        sb.append(c);
        c = nextChar();
        if (c == EOF || c == '\r' || c == '\n' || c == '\0') break;
        if (blank && (c == ' ' || c == '\t')) break;
    }
    return sb.toString();
}

std::string Scanner::next() {
    return nextStr(true);
}

int Scanner::nextInt() {
    return std::stoi(next());
}

std::string Scanner::readLine() {
    return nextStr(false);
}