cmake_minimum_required(VERSION 3.23)

project(Agents)
# List all source files for the "Agents" library
set(SOURCE_FILES PacingAgent.hpp)

# Create a library target for "Agents"

# Optionally, add any additional configuration as needed
add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})
# Create a library target for the Core component
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)