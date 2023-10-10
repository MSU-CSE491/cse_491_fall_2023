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
  # Configure SFML
  set(SFML_SRC_DIR third_party/SFML)
  set(SFML_BUILD_DIR sfml_build)
  add_subdirectory(${SFML_SRC_DIR} ${SFML_BUILD_DIR})

  # Find all the main files for the various applications
  # Currently this means any *.cpp file in the root of source
  file(GLOB EXECUTABLE_SOURCES CONFIGURE_DEPENDS RELATIVE ${CMAKE_SOURCE_DIR}/source source/*.cpp)


  # Loop through each executable and build it! 
  foreach(EXECUTABLE_SOURCE ${EXE_NAME_SOURCE_LIST})
      # Rip the .cpp off the end of the string
      string(REPLACE "_main.cpp" "" EXECUTABLE_NAME ${EXECUTABLE_NAME_SOURCE})
      # Create list of source files (currently just the one .cpp file)
      set(SOURCE_LIST source/${EXECUTABLE_SOURCE})
      # Create executable and link to includes / libraries
      add_executable(${EXECUTABLE_NAME} ${SOURCE_LIST})
      target_include_directories(${EXECUTABLE_NAME}
        PRIVATE ${CMAKE_SOURCE_DIR}
        )
      target_link_libraries(${EXECUTABLE_NAME}
        PRIVATE sfml-window sfml-audio sfml-graphics
        )
  endforeach()
endif()

# Build the test executables, if requested
if(${BUILD_TESTS})
  # Configure Catch
  set(CATCH_SRC_DIR third_party/Catch2)
  set(CATCH_BUILD_DIR catch_build)
  add_subdirectory(${CATCH_SRC_DIR} ${CATCH_BUILD_DIR})

  # Tunnel into test directory CMake infrastructure
  add_subdirectory(tests)
endif()
