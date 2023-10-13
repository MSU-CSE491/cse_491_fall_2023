cmake_minimum_required(VERSION 3.10) 
#project(tests-unit VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(STRINGS targets.txt TARGETS)

set(MAIN_SOURCE_DIR ../${MAIN_SOURCE_DIR})

foreach(TARGET IN LISTS TARGETS)
  add_executable(tests-unit-core-${TARGET} ${TARGET}.cpp
          AgentDataTest.cpp
          GameDataTest.cpp)

  target_include_directories(tests-unit-core-${TARGET}
    PRIVATE ${MAIN_SOURCE_DIR}
    )
  target_link_libraries(tests-unit-core-${TARGET}
    PRIVATE Catch2::Catch2WithMain
    )
endforeach()
