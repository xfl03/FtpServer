#include "Scanner.h"

#include "StringBuilder.h"

Scanner::Scanner(InputStream *is) {
    this->is = is;
}

std::string Scanner::next() {
    StringBuilder sb;
    char c;
    while (true) {
        c = (char) is->read();
        if (c == EOF || c == '\r' || c == '\n' || c == ' ' || c == '\t' || c == '\0') {
            break;
        }
        sb.append(c);
    }
    return sb.toString();
}