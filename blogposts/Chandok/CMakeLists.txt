cmake_minimum_required(VERSION 3.26)
project(Blog)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)

set(SOURCE_FILES main.cpp)

add_executable(Blog ${SOURCE_FILES})

target_link_libraries(Blog PRIVATE ${OpenCV_LIBS})
