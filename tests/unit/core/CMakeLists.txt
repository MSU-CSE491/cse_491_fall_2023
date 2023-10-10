cmake_minimum_required(VERSION 3.10)
#project(tests-unit VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(STRINGS targets.txt TARGETS)

set(MAIN_SOURCE_DIR ../${MAIN_SOURCE_DIR})

foreach(TARGET IN LISTS TARGETS)
  # Current directory is cmake-build-<type>
  # ^ you can check this with execute(COMMAND pwd)
  # I think this is because the top-level cmake file runs the script, but I have no idea
  if(EXISTS "../source/core/${TARGET}.cpp")
    # Add implementation file to list of source files if non-header-only class is being tested
    add_executable(tests-unit-core-${TARGET} ${TARGET}.cpp ${MAIN_SOURCE_DIR}/core/${TARGET}.cpp)
  else ()
    add_executable(tests-unit-core-${TARGET} ${TARGET}.cpp)
  endif ()

  target_include_directories(tests-unit-core-${TARGET}
          PRIVATE ${MAIN_SOURCE_DIR}
  )
  target_link_libraries(tests-unit-core-${TARGET}
          PRIVATE Catch2::Catch2WithMain
  )
endforeach()
