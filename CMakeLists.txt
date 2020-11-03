cmake_minimum_required(VERSION 3.17)
project(EE450_Socket)

set(CMAKE_CXX_STANDARD 14)

add_executable(backendA backend-serverA.cpp)
add_executable(backendB backend-serverB.cpp)
add_executable(servermain servermain.cpp)
add_executable(client1 client1.cpp)
add_executable(client2 client2.cpp)



