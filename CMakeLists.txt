cmake_minimum_required(VERSION 3.14)
project(QtNetworkExample)

# Find the required Qt packages
find_package(QT NAMES Qt6 REQUIRED COMPONENTS Core Network Widgets Qml Quick)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Network Widgets Qml Quick)


# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable AUTOMOC
set(CMAKE_AUTOMOC ON)

# Include directories for shared classes
include_directories(${CMAKE_SOURCE_DIR})

# Include your QML files
qt6_add_resources(QML_RESOURCES "res.qrc")

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
    ${QML_RESOURCES}
    ${SHARED_SRC}
)
target_link_libraries(client Qt6::Core Qt6::Network Qt6::Widgets Qt6::Qml Qt6::Quick)

# Server executable
add_executable(server
    main_server.cpp
    ${SHARED_SRC}
)
target_link_libraries(server Qt6::Core Qt6::Network)
