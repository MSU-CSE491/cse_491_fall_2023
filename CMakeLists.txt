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

#all the possible executables [each team can have their own configuration]
#copy over simple or same and change the accordingly
set(EXECUTABLES simple sfml_example group_6)


#setting the output path to executable
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/executable)


#moving assets to build
file(COPY ./assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})


#Configure SFML
include_directories(/Users/mihai/Desktop/CSE 491/GROUP6/third_party/SFML/extlibs/headers)
set(SFML_SRC_DIR third_party/SFML)
set(SFML_BUILD_DIR sfml_build)
set(MAIN_SOURCE_DIR ./)
add_subdirectory(${SFML_SRC_DIR} ${SFML_BUILD_DIR})


foreach(EXECUTABLE ${EXECUTABLES})
set(SOURCES source/${EXECUTABLE}_main.cpp
source/Worlds/BiomeGenerator.cpp)
add_executable(${EXECUTABLE} ${SOURCES})
target_include_directories(${EXECUTABLE}
PRIVATE ${MAIN_SOURCE_DIR} /Users/mihai/Desktop/CSE 491/GROUP6/third_party/SFML/extlibs/headers
)
target_link_libraries(${EXECUTABLE}
PRIVATE sfml-window sfml-audio sfml-graphics
)
endforeach()


add_subdirectory(tests)