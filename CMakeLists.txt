cmake_minimum_required(VERSION 3.16)
project(FTP_Server)

set(CMAKE_CXX_STANDARD 20)

set(BASE_SOURCES net/Socket.h net/Socket.cpp net/ServerSocket.cpp net/ServerSocket.h io/FileInputStream.cpp io/FileInputStream.h io/FileOutputStream.cpp io/FileOutputStream.h util/Scanner.cpp util/Scanner.h io/PrintWriter.cpp io/PrintWriter.h util/StringBuilder.cpp util/StringBuilder.h net/InetAddress.h net/InetAddress.cpp)
set(COMMON_SOURCES FileHelper.cpp FileHelper.h DataChannel.cpp DataChannel.h)

add_executable(ftpd ${BASE_SOURCES} ${COMMON_SOURCES} FtpServer.cpp ControlChannel.cpp ControlChannel.h)
add_executable(ftp ${BASE_SOURCES} ${COMMON_SOURCES})

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(ftpd Threads::Threads)
target_link_libraries(ftp Threads::Threads)