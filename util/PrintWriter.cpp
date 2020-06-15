#include "PrintWriter.h"

PrintWriter::PrintWriter(OutputStream *os) {
    this->os = os;
}

void PrintWriter::write(const std::string& str) {
    os->write(str.c_str(), 0, str.length());
}