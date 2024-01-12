cmake_minimum_required(VERSION 3.14)
project(StudentManager LANGUAGES CXX)

# Find the required Qt packages
find_package(QT NAMES Qt6 REQUIRED COMPONENTS Core Network Widgets Qml Quick)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Network Widgets Qml Quick)

add_definitions(
  -DSTUDENTMANAGER_DOMAIN_NAME="\"com.himanshu.${PROJECT_LOWERCASE_NAME}\""
  -DQT_USE_QSTRINGBUILDER
  -DQT_NO_CAST_TO_ASCII
  -DQT_STRICT_ITERATORS
  -DQT_NO_URL_CAST_FROM_STRING
  -DQT_NO_CAST_FROM_BYTEARRAY
  -DQT_NO_SIGNALS_SLOTS_KEYWORDS
  -DQT_USE_FAST_OPERATOR_PLUS
 )


# Set the C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable Auto MOC, UIC, and RCC for Qt
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Find the required Qt packages
find_package(Qt6 REQUIRED COMPONENTS Core Qml Quick Sql)

# Include your QML files
qt_add_resources(QML_RESOURCES "res.qrc")

# Add your source files
add_executable(StudentManager
    main.cpp
    studentmodel.cpp
    ${QML_RESOURCES}
)

# Include your header files for better IDE support
target_sources(StudentManager PRIVATE
    studentmodel.h
)

# Link your executable with the required Qt libraries
target_link_libraries(StudentManager
    Qt6::Core
    Qt6::Qml
    Qt6::Quick
    Qt6::Sql
)
