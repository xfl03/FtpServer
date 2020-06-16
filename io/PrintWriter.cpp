#include "PrintWriter.h"

PrintWriter::PrintWriter(FileOutputStream *os) {
    this->os = os;
}

void PrintWriter::write(std::string str) {
    os->write(str.c_str(), 0, str.length());
}

void PrintWriter::println(std::string str) {
    write(str + "\r\n");
}