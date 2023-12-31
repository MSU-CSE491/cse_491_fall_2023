set(TEST_PREFIX "${TEST_PREFIX}-unit")
set(PEGTL_INCLUDE_DIR ../${PEGTL_INCLUDE_DIR})

# Setup json library in tests
set(JSON_SRC_DIR ../third_party/json)
set(JSON_BUILD_DIR json_build)

# Helper function to simplify CMake files in test directories
function(create_tests NAME_PREFIX)
  # Find all of the .cpp files in this directory
  file(GLOB TARGET_SOURCES CONFIGURE_DEPENDS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)

  # Loop through each .cpp file and create a test target with dependencies
  foreach(TARGET_SOURCE IN LISTS TARGET_SOURCES)

    string(REPLACE ".cpp" "" TARGET_NAME ${TARGET_SOURCE})
    set(EXE_NAME "${NAME_PREFIX}-${TARGET_NAME}")
    message(STATUS "Building tests for: ${EXE_NAME}")
    
    # Create target

    #add_executable(${EXE_NAME} ${TARGET_SOURCE})


    add_executable(${EXE_NAME} ${TARGET_SOURCE} ${CMAKE_SOURCE_DIR}/source/core/Entity.cpp)
    

    # Load in .cmake file for target, if it exists
    if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${TARGET_NAME}.cmake)
      message(STATUS "Loading ${TARGET_NAME}.cmake")
      include(${CMAKE_CURRENT_SOURCE_DIR}/${TARGET_NAME}.cmake)
    else()
      message(WARNING "Cannot find ${TARGET_NAME}.cmake")
    endif()

    # Add includes and libraries
    target_include_directories(${EXE_NAME}
      PRIVATE ${CMAKE_SOURCE_DIR}/source ${PEGTL_INCLUDE_DIR}
      PRIVATE ${CMAKE_SOURCE_DIR}/third_party/SFML/include
      )
    target_link_libraries(${EXE_NAME}
      PRIVATE Catch2::Catch2WithMain nlohmann_json::nlohmann_json
      PRIVATE  sfml-network
      PRIVATE tinyxml2
      PRIVATE nlohmann_json::nlohmann_json
      )

    # Add to ctest
    add_test(NAME ${EXE_NAME} COMMAND ${EXE_NAME} WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
  endforeach()
endfunction()

# Process all subdirectories
add_subdirectory(core)

add_subdirectory(Agents)
add_subdirectory(Worlds)
add_subdirectory(Interfaces)
add_subdirectory(DataCollection)
add_subdirectory(XML_formater)
