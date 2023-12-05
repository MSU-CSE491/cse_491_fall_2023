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
	
	SECTION("String rule"){
		PEGTL_GRAMMAR_CHECK(worldlang::string);
		
		PARSE_TRUE(worldlang::string, "\"1234\"");
		PARSE_TRUE(worldlang::string, "\"agasad\"");
		PARSE_TRUE(worldlang::string, "\"74s76\"");
		PARSE_FALSE(worldlang::string, "abcdef");
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
		PARSE_TRUE(worldlang::expression, "\"A\"+\"B\"");
		
		PARSE_FALSE(worldlang::expression, "-iden");
	}
	
	SECTION("Assignment rules"){
		PEGTL_GRAMMAR_CHECK(worldlang::assignment);
		
		PARSE_TRUE(worldlang::expression, "a=(((3))+5)+5");
		PARSE_TRUE(worldlang::expression, "b=4-6+5");
		PARSE_TRUE(worldlang::expression, "b=\"STR\"");
	}
	
	SECTION("Block rules"){
		PEGTL_GRAMMAR_CHECK(worldlang::code_block);
		
		PARSE_TRUE(worldlang::code_block, "{\ntest(a,b)\n}\n");
		PARSE_TRUE(worldlang::code_block, "{\nb=4-6+5\n}\n");
		PARSE_TRUE(worldlang::code_block, "{\nb=\"STR\"\n}\n");
		
		PARSE_FALSE(worldlang::code_block, "{\nb=\"STR\"\n}");
	}
	
	SECTION("Comparison rules"){
		PEGTL_GRAMMAR_CHECK(worldlang::expression);
		
		PARSE_TRUE(worldlang::expression, "6+3<=10");
		PARSE_TRUE(worldlang::expression, "6+3>=5");
		PARSE_TRUE(worldlang::expression, "1!=3");
		PARSE_TRUE(worldlang::expression, "\"A\"==\"B\"");
		PARSE_TRUE(worldlang::expression, "1<2");
		PARSE_TRUE(worldlang::expression, "1>2");
		
		PARSE_FALSE(worldlang::expression, "(A=B)");
	}
	
	SECTION("Program rules"){
		PEGTL_GRAMMAR_CHECK(worldlang::program);
		
		PARSE_TRUE(worldlang::program, "a=3+5\n");
		PARSE_TRUE(worldlang::program, "func(3+func2(5))\n");
		PARSE_TRUE(worldlang::program, "b=64\nfunc(b,23)\n");
		PARSE_TRUE(worldlang::program, "b=1+2*3/4-5\nc=(8*5)+6-7*(42/7+0)\nfunc(func2(b),func3(c))\n");
		
		PARSE_TRUE(worldlang::program, "if(1){\nb=\"STR\"\n}\n");
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
	for (size_t i = 0; i < args.size(); ++i){
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
	
	//TODO: Figure out why this is slow?
	SECTION("Multiple return into multiple args"){
		PROGRAM_RUN("a=sum(seq(5))\n"){
			CHECK(pe.var<double>("a") == 15.0);
		}
	}
	
	SECTION("Allow blank lines"){
		PROGRAM_RUN("a=5\n\nb=5\n"){
			CHECK(pe.var<double>("a") == 5.0);
			CHECK(pe.var<double>("b") == 5.0);
		}
	}
	
	SECTION("String assignment + empty string"){
		PROGRAM_RUN("S=\"Hello, World!\"\nE=\"\"\n"){
			CHECK(pe.var<std::string>("S") == "Hello, World!");
			CHECK(pe.var<std::string>("E") == "");
		}
	}
	
	SECTION("Code blocks"){
		PROGRAM_RUN("if(1){\nS=\"Hello, World!\"\nE=\"\"\n}\n"){
			CHECK(pe.var<std::string>("S") == "Hello, World!");
			CHECK(pe.var<std::string>("E") == "");
		}
	}
	
	SECTION("if block"){
		PROGRAM_RUN("if(1){\na=3\n}\nb=2\nif(0){\nb=6\n}\n"){
			CHECK(pe.var<double>("a") == 3.0);
			CHECK(pe.var<double>("b") == 2.0);
		}
	}
	
	SECTION("for block"){
		PROGRAM_RUN("s=0\nfor(i,1,10){\ns=s+i\n}\n"){
			CHECK(pe.var<double>("s") == 55.0);
			CHECK(pe.var<double>("i") == 10.0);
		}
	}
	
	SECTION("for block with step"){
		PROGRAM_RUN("s=0\nfor(i,2,10,2){\ns=s+i\n}\n"){
			CHECK(pe.var<double>("s") == 30.0);
			CHECK(pe.var<double>("i") == 10.0);
		}
	}
	
	SECTION("for block with negative step"){
		PROGRAM_RUN("s=0\nfor(i,10,-10,-2){\ns=s+i\n}\n"){
			CHECK(pe.var<double>("s") == 0.0);
			CHECK(pe.var<double>("i") == -10.0);
		}
	}
	
	SECTION("for block with invalid condition"){
		PROGRAM_RUN("s=3\nfor(i,0,-1){\ns=6\n}\n"){
			CHECK(pe.var<double>("s") == 3.0);
			CHECK(pe.var<double>("i") == 0.0);
		}
	}
	
	SECTION("nested for block"){
		PROGRAM_RUN("s=0\nfor(i,0,10){\nfor(j,0,10){\ns=s+i+j\n}\n}\n"){
			CHECK(pe.var<double>("s") == 1210.0);
			CHECK(pe.var<double>("i") == 10.0);
			CHECK(pe.var<double>("j") == 10.0);
		}
	}
	
	SECTION("comparison operations"){
		PROGRAM_RUN("s=5==5\nx=3<0\n"){
			CHECK(pe.var<double>("s") == 1.0);
			CHECK(pe.var<double>("x") == 0.0);
		}
	}
	
	SECTION("User defined function"){
		PROGRAM_RUN("s(){\na=5\n}\na=0\ns()\n"){
			CHECK(pe.var<double>("a") == 5);
		}
	}
	
	SECTION("User defined function with arguments"){
		PROGRAM_RUN("s2(){\na=5\n}\ns(a,b){\nc=a\nd=b\ns2()\n}\ns(1,3)\n"){
			CHECK(pe.var<double>("c") == 1.0);
			CHECK(pe.var<double>("d") == 3.0);
			CHECK(pe.var<double>("a") == 5.0); // assigned in s by s2
			CHECK(pe.var<double>("b") == 3.0); // assigned via function call
		}
	}
	
	SECTION("User defined function (recursive)"){
		PROGRAM_RUN("s=0\nf(x){\nif(x>0){\ns=s+x\nf(x-1)\n}\n}\nf(5)\n"){
			CHECK(pe.var<double>("s") == 15.0);
			CHECK(pe.var<double>("x") == 0.0);
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

TEST_CASE("Space stripping", "[Langauge]"){
	SECTION("Check if spaces were removed"){
		CHECK(worldlang::stripWhitespace("  func (a, b)") == "func(a,b)"); // simple check
		CHECK(worldlang::stripWhitespace("if(1) {\n\ttest()  \n}\n") == "if(1){\ntest()\n}\n"); // doesn't strip newlines
		CHECK(worldlang::stripWhitespace("print( \"Hello world\" )   ") == "print(\"Hello world\")"); // doesn't remove spaces from strings
	}
}
