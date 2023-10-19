# Filename should match the application's, just swapping .cpp with .cmake
# Example: The CMake file for my_main.cpp would be my_main.cmake in the same directory

# Here, add one .cpp per line, wrapped in quotes. No commas needed
set(CPP_SOURCES 
  "source/core/Test.cpp"
  "source/core/Test2.cpp"
  )
 
# This call should not change. It's a helper function defined in the root CMakeLists file
add_sources_to_target(${EXE_NAME} ${CPP_SOURCES})
