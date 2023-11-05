# matplotpp/CMakeLists.txt
add_library(MatplotPlus MatPlotPlus.cpp)

target_include_directories(MatplotPlus PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})