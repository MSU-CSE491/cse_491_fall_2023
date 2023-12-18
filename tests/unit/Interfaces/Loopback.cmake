# Filename should match the application's, just swapping .cpp with .cmake
# Example: The CMake file for my_main.cpp would be my_main.cmake in the same directory


# Load in SFML networking
target_link_libraries(${EXE_NAME}
  PRIVATE  sfml-network
)
target_include_directories(${EXE_NAME}
  PRIVATE ${CMAKE_SOURCE_DIR}/third_party/SFML/include
)

