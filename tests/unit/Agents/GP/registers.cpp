/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief Unit tests for agent.hpp in source/Agents/GP/registers.hpp
 **/

// Catch2
#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

// Class project


#include "Agents/GP/GPAgentsRegisters.hpp"



TEST_CASE("Agent initialization", "[gp_agents]")
{

    SECTION("Initialize")
    {
        cowboys::GPAgentRegisters registers = cowboys::GPAgentRegisters();
    }

    cowboys::GPAgentRegisters registers = cowboys::GPAgentRegisters();
    SECTION("Set and get registers") {
        CHECK(registers.setRegister(0, 1));
        CHECK(registers.getRegister(0) == 1);
    }

    SECTION("Set out-of-bounds register") {
        CHECK_FALSE(registers.setRegister(20, 1)); // Should fail, as index 20 is out of bounds
    }

    SECTION("Get out-of-bounds register"){
    try {
      auto result = registers.getRegister(20);
      REQUIRE_FALSE(result.has_value());
      

    } catch (const std::out_of_range &e) {
      SUCCEED("Caught expected out_of_range exception: " + std::string(e.what()));
    }


  }



  SECTION("Check initial values") {
        for (int i = 0; i < registers.getNumRegisters(); ++i) {
            CHECK(registers.getRegister(i) == 0); // All registers should be initialized to 0
        }
    }

    SECTION("Check number of registers") {
        CHECK(registers.getNumRegisters() == 16);
    }

    SECTION("Modify multiple registers") {
        CHECK(registers.setRegister(2, 10));
        CHECK(registers.setRegister(5, 20));
        CHECK(registers.setRegister(10, 30));
        CHECK(registers.getRegister(2) == 10);
        CHECK(registers.getRegister(5) == 20);
        CHECK(registers.getRegister(10) == 30);
    }

//    SECTION("Check ostream operator") {
//        std::ostringstream output;
//        output << registers;
//        std::string expectedOutput;
//        for (int i = 0; i < registers.getNumRegisters(); ++i) {
//            expectedOutput += "Register " + std::to_string(i) + ": 0\n";
//        }
//        CHECK(output.str() == expectedOutput);
//    }


}


TEST_CASE("Test iterator functionality", "[GPAgentRegisters]") {
    cowboys::GPAgentRegisters registers;

    // Set values in registers
    for (size_t i = 0; i < registers.getNumRegisters(); ++i) {
        registers.setRegister(i, i * 10);
    }

    SECTION("Iterating through registers using iterators") {
        size_t expectedValue = 0;
        for (auto it = registers.begin(); it != registers.end(); ++it) {
            REQUIRE(*it == expectedValue);
            expectedValue += 10;
        }
    }

    SECTION("Range-based for loop") {
        size_t expectedValue = 0;
        for (const auto& value : registers) {
            REQUIRE(value == expectedValue);
            expectedValue += 10;
        }
    }
}
