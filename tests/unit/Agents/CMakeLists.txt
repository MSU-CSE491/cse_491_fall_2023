cmake_minimum_required(VERSION 3.10)
# project(tests-unit VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(STRINGS targets.txt TARGETS)

set(MAIN_SOURCE_DIR ../${MAIN_SOURCE_DIR})

# Makes target_sources() take relative paths
cmake_policy(SET CMP0076 NEW)

set(MODULE_TYPE "Agents")

foreach(TARGET IN LISTS TARGETS)
    add_executable(tests-unit-${MODULE_TYPE}-${TARGET} ${TARGET}.cpp)

    # Takes an ABSOLUTE PATH to check for file existence
    if(EXISTS "${ABSOLUTE_SOURCE_DIR}/${MODULE_TYPE}/${TARGET}.cpp")
        # Add implementation file to list of source files if non-header-only class is being tested
        # Note this is a RELATIVE PATH
        target_sources(tests-unit-${MODULE_TYPE}-${TARGET} PRIVATE ${MAIN_SOURCE_DIR}/${MODULE_TYPE}/${TARGET}.cpp)
    endif ()

    target_include_directories(tests-unit-${MODULE_TYPE}-${TARGET}
        PRIVATE ${MAIN_SOURCE_DIR}
    )
    target_link_libraries(tests-unit-${MODULE_TYPE}-${TARGET}
        PRIVATE Catch2::Catch2WithMain
    )
endforeach()
