cmake_minimum_required(VERSION 3.12)
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

project(CSE_491)

# For now, default to building both the main app and tests
set(BUILD_MAIN 1)
set(BUILD_TESTS 1)
if ("${CMAKE_BUILD_TYPE}" STREQUAL "Test")
    set(BUILD_MAIN 0)
    set(BUILD_TESTS 1)
endif ()

# Create a function to make .cmake files simpler
function(add_source_to_target TARGET_NAME SOURCE_PATH)
    message(STATUS "Loading source: ${SOURCE_PATH}")
    target_sources(${TARGET_NAME}
            PRIVATE ${CMAKE_SOURCE_DIR}/${SOURCE_PATH}
    )
endfunction()

# Set the necessary C++ flags, some of which are configuration-specific
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wcast-align -Winfinite-recursion -Wnon-virtual-dtor -Wnull-dereference -Woverloaded-virtual -pedantic")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
set(CMAKE_CXX_FLAGS_MINSIZEREL "-DNDEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")
set(CMAKE_CXX_FLAGS_COVERAGE "-O2 -g -fcoverage-mapping -fprofile-instr-generate -fprofile-arcs")

# Place all executables in the executable directory
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/executable)

# Move assets to build directory
file(COPY ./assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})


#option(BUILD_CLANG_LLVM "Build with clang for LLVM" OFF)
option(SANITIZE_MEMORY "Build with sanitizers for GP" OFF)
if (SANITIZE_MEMORY)

    set(BUILD_MAIN 0)
    set(BUILD_TESTS 0)

    set(CMAKE_CXX_COMPILER "/opt/homebrew/opt/llvm/bin/clang++")

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=memory")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=memory")

    set(XML_SRC_DIR third_party/tinyxml2)
    set(XML_BUILD_DIR xml_build)
    add_subdirectory(${XML_SRC_DIR} ${XML_BUILD_DIR})

    #added the executable
    add_executable(gp_train_main source/gp_train_main.cpp)

    #linking the targets
    target_sources(gp_train_main PRIVATE source/core/Entity.cpp)
    target_include_directories(gp_train_main
            PRIVATE ${CMAKE_SOURCE_DIR}/source/core
            ${CMAKE_SOURCE_DIR}/source/Agents
    )
    target_link_libraries(gp_train_main
            PRIVATE tinyxml2
            PRIVATE pthread
    )


endif ()





# Build the gp_train_main executable, if requested without SFML and Catch2
option(BUILD_GP_ONLY "Build only gp_main.cpp" OFF)
if (BUILD_GP_ONLY)
    set(BUILD_MAIN 0)
    set(BUILD_TESTS 0)

    set(XML_SRC_DIR third_party/tinyxml2)
    set(XML_BUILD_DIR xml_build)
    add_subdirectory(${XML_SRC_DIR} ${XML_BUILD_DIR})

    # List of executables
    set(EXECUTABLES gp_train_main gp_selective_runner_main gp_train_cgp_main gp_train_lgp_main)

    # Common source files and include directories
    set(COMMON_SOURCES source/core/Entity.cpp)
    set(COMMON_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/source/core ${CMAKE_SOURCE_DIR}/source/Agents)

    # Common libraries to link with
    set(COMMON_LIBRARIES tinyxml2 pthread)

    # Loop to configure each executable
    foreach(EXECUTABLE IN LISTS EXECUTABLES)
        add_executable(${EXECUTABLE} source/${EXECUTABLE}.cpp)
        target_sources(${EXECUTABLE} PRIVATE ${COMMON_SOURCES})
        target_include_directories(${EXECUTABLE} PRIVATE ${COMMON_INCLUDE_DIRS})
        target_link_libraries(${EXECUTABLE} PRIVATE ${COMMON_LIBRARIES})
    endforeach()

endif ()

# Typically you don't care so much for a third party library's tests to be
# run from your own project's code.
set(JSON_BuildTests OFF CACHE INTERNAL "")
# Configure json
set(JSON_SRC_DIR third_party/json)
set(JSON_BUILD_DIR json_build)
add_subdirectory(${JSON_SRC_DIR} ${JSON_BUILD_DIR})

# Build the test executables, if requested
if (${BUILD_TESTS})

    # Configure Catch
    set(CATCH_SRC_DIR third_party/Catch2)
    set(CATCH_BUILD_DIR catch_build)
    add_subdirectory(${CATCH_SRC_DIR} ${CATCH_BUILD_DIR})


    # Configure only the networking portion of SFML
    if (NOT ${BUILD_MAIN})
        set(SFML_BUILD_WINDOW FALSE)
        set(SFML_BUILD_GRAPHICS FALSE)
        set(SFML_BUILD_AUDIO FALSE)
        set(SFML_SRC_DIR third_party/SFML)
        set(SFML_BUILD_DIR sfml_build)
        add_subdirectory(${SFML_SRC_DIR} ${SFML_BUILD_DIR})

        set(XML_SRC_DIR third_party/tinyxml2)
        set(XML_BUILD_DIR xml_build)
        add_subdirectory(${XML_SRC_DIR} ${XML_BUILD_DIR})


    endif ()


    # Setup CTest
    include(CTest)
    enable_testing()

    # Tunnel into test directory CMake infrastructure
    add_subdirectory(tests)
endif ()

if (${BUILD_MAIN})
#    line to set santizers
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=[sanitizer]")
#     can be set to address, undefined, thread, memory, or leak

# Enable the SFML interface (graphics unavailable for tests on GitHub)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DUSE_SFML_INTERFACE")

  # Configure all of SFML
  set(SFML_SRC_DIR third_party/SFML)
  set(SFML_BUILD_DIR sfml_build)
  add_subdirectory(${SFML_SRC_DIR} ${SFML_BUILD_DIR})

  # Configure PEGTL
  set(PEGTL_INCLUDE_DIR third_party/PEGTL/include)

  #configure the xml library
  set(XML_SRC_DIR third_party/tinyxml2)
  set(XML_BUILD_DIR xml_build)
  add_subdirectory(${XML_SRC_DIR} ${XML_BUILD_DIR})


  # Find all the main files for the various applications
  # Currently this means any *.cpp file in the root of source
  file(GLOB EXE_SOURCES CONFIGURE_DEPENDS RELATIVE ${CMAKE_SOURCE_DIR}/source source/*.cpp)
  message(STATUS "List of main files to build: ${EXE_SOURCES}")



  # Loop through each executable and build it!
  foreach (EXE_SOURCE ${EXE_SOURCES})
      # Rip the .cpp off the end of the string
      string(REPLACE ".cpp" "" EXE_NAME ${EXE_SOURCE})
      # Create list of source files (currently just the one .cpp file)
      # Create executable and link to includes / libraries
      add_executable(${EXE_NAME} ${CMAKE_SOURCE_DIR}/source/${EXE_SOURCE} ${CMAKE_SOURCE_DIR}/source/core/Entity.cpp)
      target_include_directories(${EXE_NAME}
              PRIVATE ${CMAKE_SOURCE_DIR}/source ${PEGTL_INCLUDE_DIR}
              )
      target_link_libraries(${EXE_NAME}
              PRIVATE sfml-window sfml-audio sfml-graphics sfml-system sfml-network
    nlohmann_json::nlohmann_json
              )

      target_link_libraries(${EXE_NAME}
              PRIVATE tinyxml2
              )

      if (EXISTS ${CMAKE_SOURCE_DIR}/source/${EXE_NAME}.cmake)
          message(STATUS "Loading ${EXE_NAME}.cmake")
          include(${CMAKE_SOURCE_DIR}/source/${EXE_NAME}.cmake)
      else ()
          message(WARNING "Cannot find ${EXE_NAME}.cmake")
      endif ()
  endforeach ()
endif ()
