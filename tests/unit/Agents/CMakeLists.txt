cmake_minimum_required(VERSION 3.10)
#project(tests-unit VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(STRINGS targets.txt TARGETS)

set(MAIN_SOURCE_DIR ../${MAIN_SOURCE_DIR})

cmake_policy(SET CMP0076 NEW)

foreach(TARGET IN LISTS TARGETS)
    add_executable(tests-unit-agents-${TARGET} ${TARGET}.cpp)

    # Takes an ABSOLUTE PATH to check for file existence
    if(EXISTS "${CMAKE_SOURCE_DIR}/source/Agents/${TARGET}.cpp")
        # Add implementation file to list of source files if non-header-only class is being tested
        # Note this is a RELATIVE PATH
        target_sources(tests-unit-agents-${TARGET} PUBLIC ${MAIN_SOURCE_DIR}/Agents/${TARGET}.cpp)
    endif ()

    target_include_directories(tests-unit-agents-${TARGET}
            PRIVATE ${MAIN_SOURCE_DIR}
    )
    target_link_libraries(tests-unit-agents-${TARGET}
            PRIVATE Catch2::Catch2WithMain
    )
endforeach()
