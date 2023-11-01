set(TEST_PREFIX "${TEST_PREFIX}-agents")

# Call helper function from CMake file in tests directory
create_tests(${TEST_PREFIX})

# Tunnel into the genetic programming agents
add_subdirectory(GP)
