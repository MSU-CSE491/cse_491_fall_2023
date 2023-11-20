# Filename should match the application's, just swapping .cpp with .cmake
# Example: The CMake file for my_main.cpp would be my_main.cmake in the same directory

# Here, add one .cpp per line. Only the strings should
add_source_to_target(${EXE_NAME} "source/Interfaces/MainInterface.cpp")
add_source_to_target(${EXE_NAME} "source/Interfaces/TextureHolder.cpp")
add_source_to_target(${EXE_NAME} "source/Interfaces/Component.cpp")
add_source_to_target(${EXE_NAME} "source/Interfaces/Component.hpp")
add_source_to_target(${EXE_NAME} "source/Interfaces/Container.cpp")
add_source_to_target(${EXE_NAME} "source/Interfaces/Container.hpp")
add_source_to_target(${EXE_NAME} "source/Interfaces/Button.cpp")
add_source_to_target(${EXE_NAME} "source/Interfaces/Menu.cpp")
