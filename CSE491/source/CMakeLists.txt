cmake_minimum_required(VERSION 3.23)
project(source)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(APPLICATION_LIBRARY Agents core Interfaces Worlds)

# Find SFML
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Include the necessary subdirectories
foreach(lib ${APPLICATION_LIBRARY})
    add_subdirectory(${lib})
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/${lib})
endforeach()

# Main executable
set(SOURCE_FILES simple_main.cpp)

# Define the build target for the executable
add_executable(${PROJECT_NAME} WIN32 MACOSX_BUNDLE ${SOURCE_FILES})

# Link your main executable with SFML libraries and other libraries
# Link your main executable with SFML libraries
target_include_directories(${PROJECT_NAME} PRIVATE /opt/homebrew/opt/sfml/include)
target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-window sfml-system)
