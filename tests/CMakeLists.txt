cmake_minimum_required(VERSION 3.10) 
project(tests VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CATCH_SRC_DIR ../third_party/Catch2)
set(CATCH_BUILD_DIR catch_build)

# Setup json library in tests
set(JSON_SRC_DIR ../third_party/json)
set(JSON_BUILD_DIR json_build)

set(MAIN_SOURCE_DIR ../source)
set(PEGTL_INCLUDE_DIR ../third_party/PEGTL/include)

configure_file(run_tests.sh run_tests.sh COPYONLY)

# Catch2 CMake integration
# From https://github.com/catchorg/Catch2/blob/devel/docs/cmake-integration.md#top
add_subdirectory(${CATCH_SRC_DIR} ${CATCH_BUILD_DIR})

# TODO: Might have to use a different approach for running from /tests/build.
# Cannot uncomment this and link with nlohmann_json properly in both
# /build and /tests/build at the same time.
# add_subdirectory(${JSON_SRC_DIR} ${JSON_BUILD_DIR})

add_subdirectory(unit)
