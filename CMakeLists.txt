cmake_minimum_required(VERSION 3.16)
project(FTP_Server)

set(CMAKE_CXX_STANDARD 20)

add_executable(FTP_Server main.cpp net/Socket.h net/Socket.cpp net/ServerSocket.cpp net/ServerSocket.h io/InputStream.cpp io/InputStream.h io/OutputStream.cpp io/OutputStream.h util/Scanner.cpp util/Scanner.h util/PrintWriter.cpp util/PrintWriter.h util/StringBuilder.cpp util/StringBuilder.h)