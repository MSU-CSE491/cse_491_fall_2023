/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief Unit tests for source/Worlds/Language.hpp
 **/

// Catch2 
#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

// Class project
#include "Worlds/Language.hpp"

// Macros to simplify case creation
#define PEGTL_PARSE_CHECK(grammar, string, condition) {\
	pegtl::memory_input in(string, "");\
	CHECK(pegtl::parse< grammar >(in) == condition); \
}

#define PARSE_TRUE(g, s) PEGTL_PARSE_CHECK(g, s, true)
#define PARSE_FALSE(g, s) PEGTL_PARSE_CHECK(g, s, false)

TEST_CASE("Basic compilation test", "[World][Language]"){
	SECTION("Test compilation"){
		// Doesn't need to do anything really, just makes sure this is set up right.
		CHECK(true);
	}
}

TEST_CASE("Langauge check", "[World][Langauge]"){
	SECTION("Number rule"){
		PARSE_TRUE(worldlang::number, "1234");
		PARSE_TRUE(worldlang::number, "3.6");
		PARSE_TRUE(worldlang::number, "746.876");
		PARSE_TRUE(worldlang::number, "-0.4")
		PARSE_FALSE(worldlang::number, "abcd");
	}
}

