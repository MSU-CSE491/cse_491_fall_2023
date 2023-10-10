cmake_minimum_required(VERSION 3.10) 


set(MAIN_SOURCE_DIR ../${MAIN_SOURCE_DIR})
add_subdirectory(core)
add_subdirectory(gp_agents)


#enable_testing()

# Add tests for the subdirectories
#add_test(NAME CoreTests COMMAND core_tests)
#add_test(NAME GpAgentsTests COMMAND gp_agents_tests)





# Optionally, you can group tests into a test suite
#set_tests_properties(CoreTests GpAgentsTests PROPERTIES LABELS "MyTestSuite")

# Optionally, you can run all tests using a custom target
#add_custom_target(run_tests
#        COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
#        DEPENDS CoreTests GpAgentsTests
#        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
#)