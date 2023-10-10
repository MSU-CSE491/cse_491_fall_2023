set(TEST_PREFIX "${TEST_PREFIX}-worlds")
# Find all of the .cpp files in this directory
file(GLOB TARGET_SOURCES CONFIGURE_DEPENDS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)

set(MAIN_SOURCE_DIR ../${MAIN_SOURCE_DIR})

foreach(TARGET_SOURCE IN LISTS TARGET_SOURCES)
  string(REPLACE ".cpp" "" TARGET_NAME ${TARGET_SOURCE})
  set(EXE_NAME "${TEST_PREFIX}-${TARGET_NAME}")
  message(STATUS "Building tests for: ${EXE_NAME}")
  add_executable(${EXE_NAME} ${TARGET_SOURCE})

  target_include_directories(${EXE_NAME}
    PRIVATE ${CMAKE_SOURCE_DIR}/source
    )
  target_link_libraries(${EXE_NAME}
    PRIVATE Catch2::Catch2WithMain
    )
  add_test(NAME ${EXE_NAME} COMMAND ${EXE_NAME})
endforeach()
