cmake_minimum_required(VERSION 3.19)
set(CMAKE_CXX_STANDARD 20)

project(client)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(SFML_SRC_DIR ../../../../third_party/SFML)
set(SFML_BUILD_DIR sfml_build)
add_subdirectory(${SFML_SRC_DIR} ${SFML_BUILD_DIR})

add_executable(CMakeSFMLProjectClient clientmain.cpp)
target_link_libraries(CMakeSFMLProjectClient PRIVATE sfml-network)
target_compile_features(CMakeSFMLProjectClient PRIVATE cxx_std_20)

install(TARGETS CMakeSFMLProjectClient)