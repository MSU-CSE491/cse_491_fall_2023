#pragma once

// Debug
#include <iostream>

#include <tao/pegtl.hpp>

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
	
	struct op_prio_add : pegtl::one< '+', '-' >
	{};
	
	struct op_prio_mul : pegtl::one< '*', '/' >
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
	
	struct mul_a : pegtl::sor<
		pegtl::seq<
			element,
			op_prio_mul,
			element
		>,
		element
	>
	{};
	
	struct mul;
	struct mul : pegtl::sor<
		pegtl::seq<
			mul_a,
			op_prio_mul,
			mul
		>,
		mul_a
	>
	{};
	
	// Hope this works lol
	struct add_a : pegtl::sor<
		pegtl::seq<
			mul,
			op_prio_add,
			mul
		>,
		mul
	>
	{};	
	
	struct add;
	struct add : pegtl::sor<
		pegtl::seq<
			add_a,
			op_prio_add,
			add
		>,
		add_a
	>
	{};
	
	// Expressions:
	// expr :=  add | e <op> e | e <op> expr
	struct expression : pegtl::sor<
		add	
	>
	{};
	
	template <typename Rule>
	struct action : pegtl::nothing< Rule > {};
	
	// Specialize to capture numbers
	template <>
	struct action<number>
	{
		template <typename ActionInput>
		static void apply( const ActionInput& in, std::string& s){
			s += in.string() + " ";
			std::cout << s << std::endl;
		}	
	};
} //worldlang
