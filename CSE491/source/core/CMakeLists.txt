# List source files for the Core library
cmake_minimum_required(VERSION 3.23)

project(Core)
set(SOURCE_FILES
        AgentBase.hpp
        CoreObject.hpp
        Data.hpp
        Entity.hpp
        GridPosition.hpp
        InterfaceBase.hpp
        WorldBase.hpp
        WorldGrid.hpp
        )
add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})
# Create a library target for the Core component
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)