cmake_minimum_required(VERSION 3.14)
project(TCP_IP_Client)

set(CMAKE_CXX_STANDARD 14)

add_executable(TCP_IP_Client main.cpp)
target_link_libraries(TCP_IP_Client ws2_32)