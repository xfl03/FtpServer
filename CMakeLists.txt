cmake_minimum_required(VERSION 3.16)
project(FTP_Server)

set(CMAKE_CXX_STANDARD 20)

add_executable(FTP_Server main.cpp net/Socket.h net/Socket.cpp net/ServerSocket.cpp net/ServerSocket.h io/FileInputStream.cpp io/FileInputStream.h io/FileOutputStream.cpp io/FileOutputStream.h util/Scanner.cpp util/Scanner.h io/PrintWriter.cpp io/PrintWriter.h util/StringBuilder.cpp util/StringBuilder.h net/InetAddress.h net/InetAddress.cpp)