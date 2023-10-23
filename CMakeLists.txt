cmake_minimum_required(VERSION 3.10)
project(GROUP6)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(GROUP6 source/simple_main.cpp
        source/Worlds/BiomeGenerator.cpp
        source/Worlds/BiomeGenerator.hpp
        source/Worlds/PerlinNoise.hpp
        )

#target_compile_options(GROUP6 PRIVATE -Wall -Wpedantic -Werror)
