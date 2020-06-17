#include "Scanner.h"
#include <stdexcept>
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
    if (size == 0) return EOF;
    return buffer[pos++];
}

std::string Scanner::nextStr(bool blank) {
    StringBuilder sb;
    char c = nextChar();
    if (c == '\n') c = nextChar(); // Clear first LF
    if (c == EOF) return "";
    while (true) {
        sb.append(c);
        c = nextChar();
        if (c == EOF || c == '\r' || c == '\n' || c == '\0') {
            lineFinished = true;
            break;
        }
        if (blank && (c == ' ' || c == '\t')) {
            lineFinished = false;
            break;
        }
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

bool Scanner::isLineFinished() {
    return lineFinished;
}