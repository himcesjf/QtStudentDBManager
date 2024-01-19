cmake_minimum_required(VERSION 3.14)
project(QtStudentDBManager)

# Find the required Qt packages
find_package(Qt6 REQUIRED COMPONENTS Core Network Qml Quick Sql)

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
    NetworkMessage.cpp
    Student.cpp
)

# Client executable
add_executable(client
    main_client.cpp
    StudentModel.cpp
    ${QML_RESOURCES}
    ${SHARED_SRC}
)
target_link_libraries(client Qt6::Core Qt6::Network Qt6::Qml Qt6::Quick Qt6::Sql)

# Server executable
add_executable(server
    main_server.cpp
    StudentDatabase.cpp
    StudentServer.cpp
    ${SHARED_SRC}
)
target_link_libraries(server Qt6::Core Qt6::Network Qt6::Sql)
