cmake_minimum_required(VERSION 3.14)
project(QtPostgresApp)

# Find the Qt package, specifying the required components
find_package(Qt6 COMPONENTS Core Sql REQUIRED)

# Tell CMake to run moc when necessary
set(CMAKE_AUTOMOC ON)

# As moc files are generated in the binary dir, tell CMake to always look for includes there
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Define the executable
add_executable(${PROJECT_NAME} main.cpp)

# Link the Qt modules
target_link_libraries(${PROJECT_NAME} Qt6::Core Qt6::Sql)

