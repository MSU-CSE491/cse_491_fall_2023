# move a the test file into a executable directory

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/test.xml ${CMAKE_BINARY_DIR}/executable/test.xml COPYONLY)
