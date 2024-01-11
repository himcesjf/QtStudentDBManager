cmake_minimum_required(VERSION 3.10)
project(ClientServerApp)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_AUTOMOC ON)
find_package(Qt5Core REQUIRED)
find_package(Qt5Network REQUIRED)

add_executable(server server.cpp)
target_link_libraries(server Qt5::Core Qt5::Network)

add_executable(client client.cpp)
target_link_libraries(client Qt5::Core Qt5::Network)
