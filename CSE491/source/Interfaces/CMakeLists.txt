
cmake_minimum_required(VERSION 3.23)

project(Interfaces)
 # List source files for the Interfaces library
 set(SOURCE_FILES
 TrashInterface.hpp
 )

# Optionally, add any additional configuration as needed
add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})
# Create a library target for the Core component
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
