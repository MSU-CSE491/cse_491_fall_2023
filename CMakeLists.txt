cmake_minimum_required(VERSION 3.12)
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

project(CSE_491)

# For now, default to building both the main app and tests
set(BUILD_MAIN 1)
set(BUILD_TESTS 1)
if("${CMAKE_BUILD_TYPE}" STREQUAL "Test")
  set(BUILD_MAIN 0)
  set(BUILD_TESTS 1)
endif()

# Create a function to make .cmake files simpler
function(add_source_to_target TARGET_NAME SOURCE_PATH)
  message(STATUS "Loading source: ${SOURCE_PATH}")
  target_sources(${TARGET_NAME}
    PRIVATE ${CMAKE_SOURCE_DIR}/${SOURCE_PATH}
    )
endfunction()

# Set the necessary C++ flags, some of which are configuration-specific
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wcast-align -Winfinite-recursion -Wnon-virtual-dtor -Wnull-dereference -Woverloaded-virtual -pedantic")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
set(CMAKE_CXX_FLAGS_MINSIZEREL "-DNDEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")


# Place all executables in the executable directory
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/executable)

# Move assets to build directory
file(COPY ./assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# Build the main application executables, if requested
if(${BUILD_MAIN})

  # Configure all of SFML
  set(SFML_SRC_DIR third_party/SFML)
  set(SFML_BUILD_DIR sfml_build)
  add_subdirectory(${SFML_SRC_DIR} ${SFML_BUILD_DIR})


foreach(EXECUTABLE ${EXECUTABLES})
    set(SOURCES source/${EXECUTABLE}_main.cpp source/Interfaces/MainInterface.cpp source/Interfaces/Component.cpp source/Interfaces/Component.hpp source/Interfaces/Container.cpp source/Interfaces/Container.hpp source/Interfaces/Button.cpp source/Interfaces/Button.hpp)
    add_executable(${EXECUTABLE} ${SOURCES} source/Interfaces/Component.cpp source/Interfaces/Component.hpp source/Interfaces/Button.cpp source/Interfaces/Button.hpp source/Interfaces/Menu.cpp source/Interfaces/Menu.hpp)
    target_include_directories(${EXECUTABLE}
            PRIVATE ${MAIN_SOURCE_DIR}
            )
    target_link_libraries(${EXECUTABLE}
            PRIVATE sfml-window sfml-audio sfml-graphics sfml-system
            )
endforeach()

  # Configure Catch
  set(CATCH_SRC_DIR third_party/Catch2)
  set(CATCH_BUILD_DIR catch_build)
  add_subdirectory(${CATCH_SRC_DIR} ${CATCH_BUILD_DIR})

  # Configure only the networking portion of SFML
  if(NOT ${BUILD_MAIN})
    set(SFML_BUILD_WINDOW FALSE)
    set(SFML_BUILD_GRAPHICS FALSE)
    set(SFML_BUILD_AUDIO FALSE)
    set(SFML_SRC_DIR third_party/SFML)
    set(SFML_BUILD_DIR sfml_build)
    add_subdirectory(${SFML_SRC_DIR} ${SFML_BUILD_DIR})
  endif()

  # Setup CTest
  include(CTest)
  enable_testing()

  # Tunnel into test directory CMake infrastructure
  add_subdirectory(tests)
endif()
