cmake_minimum_required(VERSION 3.16)
project(FTP_Server)

set(CMAKE_CXX_STANDARD 20)

add_executable(FTP_Server main.cpp net/Socket.h net/Socket.cpp net/ServerSocket.cpp net/ServerSocket.h io/FileInputStream.cpp io/FileInputStream.h io/FileOutputStream.cpp io/FileOutputStream.h util/Scanner.cpp util/Scanner.h io/PrintWriter.cpp io/PrintWriter.h util/StringBuilder.cpp util/StringBuilder.h net/InetAddress.h net/InetAddress.cpp FileHelper.cpp FileHelper.h ControlChannel.cpp ControlChannel.h DataChannel.cpp DataChannel.h)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(FTP_Server Threads::Threads)