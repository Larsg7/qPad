cmake_minimum_required(VERSION 3.5)
project(myqtproject)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(SOURCE_FILES main.cpp qpad.cpp resource.qrc)
find_package(Qt5Widgets REQUIRED)

add_executable(myqtproject ${SOURCE_FILES})

target_link_libraries(myqtproject Qt5::Widgets)