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
set(EXECUTABLES simple sfml_example jsontest)


# setting the output path to executable
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/executable)


# moving assets to build
file(COPY ./assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# Typically you don't care so much for a third party library's tests to be
# run from your own project's code.
set(JSON_BuildTests OFF CACHE INTERNAL "")



# Configure json
set(JSON_SRC_DIR third_party/json)
set(JSON_BUILD_DIR json_build)
add_subdirectory(${JSON_SRC_DIR} ${JSON_BUILD_DIR})


# Configure SFML
set(SFML_SRC_DIR third_party/SFML)
set(SFML_BUILD_DIR sfml_build)
set(MAIN_SOURCE_DIR ./)
add_subdirectory(${SFML_SRC_DIR} ${SFML_BUILD_DIR})


foreach(EXECUTABLE ${EXECUTABLES})
    set(SOURCES source/${EXECUTABLE}_main.cpp)
    add_executable(${EXECUTABLE} ${SOURCES})
    target_include_directories(${EXECUTABLE}
      PRIVATE ${MAIN_SOURCE_DIR}
      )
    target_link_libraries(${EXECUTABLE}
      PRIVATE sfml-window sfml-audio sfml-graphics
      nlohmann_json::nlohmann_json
      )
endforeach()


add_subdirectory(tests)
