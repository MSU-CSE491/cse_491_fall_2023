cmake_minimum_required(VERSION 3.19)
set(CMAKE_CXX_STANDARD 20)

project(client)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

#include(FetchContent)
#FetchContent_Declare(SFML
#        GIT_REPOSITORY https://github.com/SFML/SFML.git
#        GIT_TAG 2.6.x)
#FetchContent_MakeAvailable(SFML)
#
#add_executable(CMakeSFMLProject clientmain.cpp)
#target_link_libraries(CMakeSFMLProject PRIVATE sfml-network)
#target_compile_features(CMakeSFMLProject PRIVATE cxx_std_20)
#
#if(WIN32)
#    add_custom_command(
#            TARGET CMakeSFMLProject
#            COMMENT "Copy OpenAL DLL"
#            PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:CMakeSFMLProject>
#            VERBATIM)
#endif()
#

set(SFML_SRC_DIR ../../../../third_party/SFML)
set(SFML_BUILD_DIR sfml_build)
add_subdirectory(${SFML_SRC_DIR} ${SFML_BUILD_DIR})

add_executable(CMakeSFMLProject clientmain.cpp)
set_target_properties(
        CMakeSFMLProject
        PROPERTIES
        OUTPUT_NAME "clientmain"
        SUFFIX ".networth"
)
target_link_libraries(CMakeSFMLProject PRIVATE sfml-network)
target_compile_features(CMakeSFMLProject PRIVATE cxx_std_20)

install(TARGETS CMakeSFMLProject)