cmake_minimum_required(VERSION 3.12)

project(CSE_491)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wcast-align -Winfinite-recursion -Wnon-virtual-dtor -Wnull-dereference -Woverloaded-virtual -pedantic")

set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
set(CMAKE_CXX_FLAGS_MINSIZEREL "-DNDEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")

# all the possible executables [each team can have their own configuration]
# copy over simple or same and change the accordingly
set(EXECUTABLES simple sample)


# setting the output path to executable
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/executable)


# moving assets to build
file(COPY ./assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})



foreach(EXECUTABLE ${EXECUTABLES})
    if(${EXECUTABLE} STREQUAL "simple")
        set(SOURCES source/${EXECUTABLE}_main.cpp)
    elseif(${EXECUTABLE} STREQUAL "sample")
        set(SOURCES source/${EXECUTABLE}_main.cpp)

    # add the executable here to complile and run
    endif()
    add_executable(${EXECUTABLE} ${SOURCES})

endforeach()


add_subdirectory(tests)