cmake_minimum_required(VERSION 3.10)
project(udprelay)

set(CMAKE_CXX_STANDARD 11)

add_executable(udprelay  UdpRelay.c)
find_package(Threads REQUIRED)
target_link_libraries(udprelay Threads::Threads)