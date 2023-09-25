# Optionally, list any source files here if you add any in the future
cmake_minimum_required(VERSION 3.23)

project(Worlds)
set(SOURCE_FILES MazeWorld.hpp)



# Optionally, add any additional configuration as needed
add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})
# Create a library target for the Core component
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
# Copy the "assets" folder from the parent directory to the build directory
