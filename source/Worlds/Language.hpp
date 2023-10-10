#pragma once

#include "../third_party/PEGTL/include/tao/pegtl.hpp"

namespace pegtl = tao::pegtl;

namespace worldlang{
	
	struct grammar : pegtl::must< pegtl::eof >
	{};
	
	// Equivalent regex
	// \-?[0-9]+(.[0-9]+)?
	struct number : pegtl::seq < 
		pegtl::opt<
			pegtl::one< '-' >
		>,
		pegtl::plus< pegtl::digit >,
		pegtl::opt<
			pegtl::one< '.' >,
			pegtl::plus< pegtl::digit >
		>
	>
	{};
	
	//[_a-zA-Z][_a-zA-Z0-9]
	// (C-style identifier, case sensitive)
	struct identifier : pegtl::identifier
	{};
	
	// Match an operator
	struct any_operator : pegtl::sor<
		pegtl::one < '+', '-', '*', '/' >,
//		pegtl::one < '&', '|', '^' >,
		TAO_PEGTL_KEYWORD("&&"),
		TAO_PEGTL_KEYWORD("||")
	>
	{};

	// Must forward-declare for recursion
	struct expression;

	// Match single value	
	struct element : pegtl::sor<
		identifier,
		number,
		pegtl::seq<
			pegtl::one< '(' >,
			expression,
			pegtl::one< ')' >
		>
	>
	{};
	
	// Expressions:
	// expr :=  e <op> e | e <op> expr
	struct expression : pegtl::seq<
		element,
		any_operator,
		element
	>
	{};
} //worldlang
