/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief Unit tests for source/Worlds/Language.hpp
 **/

// Catch2 
#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include <string>

// Class project
#include "Worlds/Language.hpp"
#include "Worlds/ProgramExecutor.hpp"

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
	
	SECTION("Program rules"){
		PEGTL_GRAMMAR_CHECK(worldlang::program);
		
		PARSE_TRUE(worldlang::program, "a=3+5\n");
		PARSE_TRUE(worldlang::program, "func(3+func2(5))\n");
		PARSE_TRUE(worldlang::program, "b=64\nfunc(b,23)\n");
		PARSE_TRUE(worldlang::program, "b=1+2*3/4-5\nc=(8*5)+6-7*(42/7+0)\nfunc(func2(b),func3(c))\n");
	}
}

using worldlang::ProgramExecutor;

// Simple function test
void square(ProgramExecutor& pe){
	auto args = pe.popArgs();
	if (args.size() != 1.0){
		pe.error("Wrong number of arguments!");
	} else {
		auto arg = pe.as<double>(args[0]);
		pe.pushStack(arg*arg);
	}
}

// Variable number of arguments test
void summation(ProgramExecutor& pe){
	auto args = pe.popArgs();
	double sum = 0;
	for (int i = 0; i < args.size(); ++i){
		auto arg = pe.as<double>(args[i]);
		
		sum += arg;
	}
	pe.pushStack(sum);
}

// Multiple return test
// Generates interger sequence 0,1,...,n
void sequence(ProgramExecutor& pe){
	auto args = pe.popArgs();
//	auto count = pe.as<double>(pe.popStack());
	if (args.size() != 1.0){
		pe.error("Wrong number of arguments!");
	} else {
		for (int i = 0; i <= pe.as<double>(args.at(0)); ++i){
			pe.pushStack(static_cast<double>(i));
		}
	}
}

// Drops "pe" into this scope
#define PROGRAM_RUN(program) \
	ProgramExecutor pe{};\
	pe.registerFunction("square", square);\
	pe.registerFunction("sum", summation);\
	pe.registerFunction("seq", sequence);\
	pe.run(program);\

using Catch::Matchers::WithinAbs;

TEST_CASE("Program execution correct", "[World][Language]"){
	SECTION("Simple assignment"){
		PROGRAM_RUN("a=5\n"){
			CHECK(pe.var<double>("a") == 5.0);
		}
	}
	
	SECTION("Multiple simple assignments"){
		PROGRAM_RUN("a=42\nb=63\n"){
			CHECK(pe.var<double>("a") == 42.0);
			CHECK(pe.var<double>("b") == 63.0);
		}
	}
	
	SECTION("Operators"){
		PROGRAM_RUN("a=42+653\nb=675-432\nc=63*234\nd=123/57\n"){
			CHECK(pe.var<double>("a") == 695.0);
			CHECK(pe.var<double>("b") == 243.0);
			CHECK(pe.var<double>("c") == 14742.0);
			CHECK_THAT(pe.var<double>("d"), WithinAbs(2.15789, 0.00001));
		}
	}
	
	SECTION("Simple function test"){
		PROGRAM_RUN("a=square(4)\nb=square(a)\n"){
			CHECK(pe.var<double>("a") == 16.0);
			CHECK(pe.var<double>("b") == 256.0);
		}
	}
	
	SECTION("Variadic function"){
		PROGRAM_RUN("a=sum(1,2,3)\nb=sum()\nc=sum(1,2,3,4,5,6,7,8,9)\n"){
			CHECK(pe.var<double>("a") == 6.0);
			CHECK(pe.var<double>("b") == 0.0);
			CHECK(pe.var<double>("c") == 45.0);
		}
	}
	
	SECTION("Multiple assignment"){
		PROGRAM_RUN("a,b=2,6\n"){
			CHECK(pe.var<double>("a") == 2.0);
			CHECK(pe.var<double>("b") == 6.0);
		}
	}
	
	SECTION("Multiple return"){
		PROGRAM_RUN("a,b,c,d,e=seq(4)\n"){
			CHECK(pe.var<double>("a") == 0.0);
			CHECK(pe.var<double>("b") == 1.0);
			CHECK(pe.var<double>("c") == 2.0);
			CHECK(pe.var<double>("d") == 3.0);
			CHECK(pe.var<double>("e") == 4.0);
		}
	}
	
	SECTION("Multiple return into multiple args"){
		PROGRAM_RUN("a=sum(seq(5))\n"){
			CHECK(pe.var<double>("a") == 15.0);
		}
	}
}

TEST_CASE("Program execution errors", "[World][Langauge]"){
	SECTION("Error check for wrong number of arguments"){
		PROGRAM_RUN("seq()\n"){
			CHECK(!pe.getErrorMessage().empty());
		}
	}
	
	SECTION("Error on wrong number of values (low)"){
		PROGRAM_RUN("a,b=1\n"){
			CHECK(!pe.getErrorMessage().empty());
		}
	}
	
	SECTION("Error on wrong number of values (high)"){
		PROGRAM_RUN("a,b=1,2,3\n"){
			CHECK(!pe.getErrorMessage().empty());
		}
	}

}
