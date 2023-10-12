/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief Unit tests for source/Worlds/Language.hpp
 **/

// Catch2 
#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include <string>

// Class project
#include "Worlds/Language.hpp"

// PEGTL grammar check
#include <tao/pegtl/contrib/analyze.hpp>

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
	
	SECTION("Addition-level operators rule"){
		PEGTL_GRAMMAR_CHECK(worldlang::op_prio_add);
		
		PARSE_TRUE(worldlang::op_prio_add, "+");
		PARSE_TRUE(worldlang::op_prio_add, "-");
		PARSE_FALSE(worldlang::op_prio_add, "*");
		PARSE_FALSE(worldlang::op_prio_add, "/");
	}
	
	SECTION("Multiplication-level operators rule"){
		PEGTL_GRAMMAR_CHECK(worldlang::op_prio_mul);
		
		PARSE_FALSE(worldlang::op_prio_mul, "+");
		PARSE_FALSE(worldlang::op_prio_mul, "-");
		PARSE_TRUE(worldlang::op_prio_mul, "*");
		PARSE_TRUE(worldlang::op_prio_mul, "/");
	}
	
	SECTION("Add rule"){
		PEGTL_GRAMMAR_CHECK(worldlang::add);
		
		PARSE_TRUE(worldlang::add, "4+4");
		// Due to precedence rules, this should work as add contains mul
		PARSE_TRUE(worldlang::add, "6*8");
	}
	
	// This section tests pretty much every big expression rule
	SECTION("Expression rules"){
		PEGTL_GRAMMAR_CHECK(worldlang::expression);
		
		PARSE_TRUE(worldlang::expression, "3+5");
		PARSE_TRUE(worldlang::expression, "3+5+6");
		PARSE_TRUE(worldlang::expression, "3*5+8");
		PARSE_TRUE(worldlang::expression, "3*6*5");
		PARSE_TRUE(worldlang::expression, "46.7+(32)");
		PARSE_TRUE(worldlang::expression, "(3+5)+5");
		PARSE_TRUE(worldlang::expression, "(((3))+5)+5");
		PARSE_TRUE(worldlang::expression, "4-6+5");
		
		PARSE_TRUE(worldlang::expression, "(3+-5)+Name");
		PARSE_TRUE(worldlang::expression, "(((3))+36+dd)+5");
		PARSE_TRUE(worldlang::expression, "asdf-asdf+asdf");
		
		PARSE_FALSE(worldlang::expression, "-iden");
	}	
}

