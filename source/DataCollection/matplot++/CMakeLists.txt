# project-root/CMakeLists.txt
cmake_minimum_required(VERSION 3.0)
project(MatplotExample)

add_subdirectory(matplotpp) # Include the matplot++ library

add_executable(MatplotExample main.cpp)
target_link_libraries(MatplotExample PRIVATE MatplotPlus)