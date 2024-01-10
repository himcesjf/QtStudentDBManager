cmake_minimum_required(VERSION 3.14)

project(QtStudentDBManager LANGUAGES CXX)

# Enable Auto MOC, UIC, and RCC
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Set the C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find the required Qt packages
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Sql Quick Qml QuickControls2)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Sql Quick Qml QuickControls2)

# Process the QML files in the resource file
qt5_add_resources(QML_RESOURCES qml.qrc) # Use qt6_add_resources for Qt6

# Define your executable and list all the relevant source and header files
add_executable(QtStudentDBManager
  main.cpp
  Student.h
  Student.cpp
  DatabaseOperations.h
  DatabaseOperations.cpp
  StudentModel.h
  StudentModel.cpp
  ${QML_RESOURCES} # Add the processed resource file to the executable
)

# Link your executable with the required Qt libraries
target_link_libraries(QtStudentDBManager
  Qt${QT_VERSION_MAJOR}::Core
  Qt${QT_VERSION_MAJOR}::Sql
  Qt${QT_VERSION_MAJOR}::Quick
  Qt${QT_VERSION_MAJOR}::Qml
  Qt${QT_VERSION_MAJOR}::QuickControls2 # Include QuickControls2 for TableView
)

# Installation settings
include(GNUInstallDirs)
install(TARGETS QtStudentDBManager
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
