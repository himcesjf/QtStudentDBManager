cmake_minimum_required(VERSION 3.14)
project(QtNetworkExample)

# Find the QtWidgets library
#find_package(Qt6 COMPONENTS Core Network Widgets REQUIRED)

# Find the required Qt packages
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Network Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Network Widgets)


# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable AUTOMOC
set(CMAKE_AUTOMOC ON)

# Include directories for shared classes
include_directories(${CMAKE_SOURCE_DIR})

# Shared source files
set(SHARED_SRC
    Student.cpp
    Student.h
)

# Client executable
add_executable(client
    main_client.cpp
    StudentModel.cpp
    StudentModel.h
    ${SHARED_SRC}
)
target_link_libraries(client Qt5::Core Qt5::Network Qt5::Widgets)

# Server executable
add_executable(server
    main_server.cpp
    ${SHARED_SRC}
)
target_link_libraries(server Qt5::Core Qt5::Network)
