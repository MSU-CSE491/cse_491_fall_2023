set(TEST_PREFIX "${TEST_PREFIX}-worlds")
set(PEGTL_INCLUDE_DIR ../${PEGTL_INCLUDE_DIR})

# Call helper function from CMake file in tests directory
create_tests(${TEST_PREFIX})
