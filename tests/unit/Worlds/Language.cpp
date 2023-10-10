/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief Unit tests for source/Worlds/Language.hpp
 **/

// Catch2 
#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

// Class project
#include "Worlds/Language.hpp"

// PEGTL grammar check
#include "../third_party/PEGTL/include/tao/pegtl/contrib/analyze.hpp"

// Macros to simplify case creation
#define PEGTL_PARSE_CHECK(grammar, string, condition) {\
	pegtl::memory_input in(string, "");\
	CHECK(pegtl::parse< grammar >(in) == condition); \
}

#define PARSE_TRUE(g, s) PEGTL_PARSE_CHECK(g, s, true)
#define PARSE_FALSE(g, s) PEGTL_PARSE_CHECK(g, s, false)

#define PEGTL_GRAMMAR_CHECK(grammar) CHECK(pegtl::analyze<grammar>(1) == 0);

TEST_CASE("Basic compilation test", "[World][Language]"){
	SECTION("Test compilation"){
		// Doesn't need to do anything really, just makes sure this is set up right.
		CHECK(true);
	}
}

TEST_CASE("Langauge check", "[World][Langauge]"){
	SECTION("Number rule"){
		PEGTL_GRAMMAR_CHECK(worldlang::number);
		
		PARSE_TRUE(worldlang::number, "1234");
		PARSE_TRUE(worldlang::number, "3.6");
		PARSE_TRUE(worldlang::number, "746.876");
		PARSE_TRUE(worldlang::number, "-0.4")
		PARSE_FALSE(worldlang::number, "abcd");
	}

	SECTION("Identifier rule"){
		PEGTL_GRAMMAR_CHECK(worldlang::identifier);
		
		PARSE_TRUE(worldlang::identifier, "abc");
		PARSE_TRUE(worldlang::identifier, "abc123");
		PARSE_TRUE(worldlang::identifier, "ABC");
		PARSE_TRUE(worldlang::identifier, "_ident");
		PARSE_TRUE(worldlang::identifier, "Z999");
		PARSE_TRUE(worldlang::identifier, "name");

		PARSE_FALSE(worldlang::identifier, "0abc");
		PARSE_FALSE(worldlang::identifier, ".abc");
		PARSE_FALSE(worldlang::identifier, "369");
		PARSE_FALSE(worldlang::identifier, "!~!");
	}

	SECTION("Operator rules"){
		PEGTL_GRAMMAR_CHECK(worldlang::any_operator);

		PARSE_TRUE(worldlang::any_operator, "+");
		PARSE_TRUE(worldlang::any_operator, "-");
		PARSE_TRUE(worldlang::any_operator, "/");
		PARSE_TRUE(worldlang::any_operator, "*");
		PARSE_TRUE(worldlang::any_operator, "||");
		PARSE_TRUE(worldlang::any_operator, "&&");

		PARSE_FALSE(worldlang::any_operator, "a");
		PARSE_FALSE(worldlang::any_operator, "0");
		PARSE_FALSE(worldlang::any_operator, "`");
	}

	SECTION("Expression rules"){
		PEGTL_GRAMMAR_CHECK(worldlang::expression);
		
		PARSE_TRUE(worldlang::expression, "3+5");
	}	
}

