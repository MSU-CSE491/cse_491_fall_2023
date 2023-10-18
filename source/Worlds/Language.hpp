#pragma once

// Debug
#include <iostream>

#include <tao/pegtl.hpp>
#include <tao/pegtl/contrib/parse_tree.hpp>

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
	struct expression_list;	
	
	// Function call
	struct function : pegtl::seq<
		identifier,
		pegtl::one< '(' >,
		pegtl::opt<expression_list>,
		pegtl::one< ')' >
	>
	{};
	
	// Match single value	
	struct element : pegtl::sor<
		function,
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
	
	struct expression_list;	
	struct expression_list : pegtl::sor<
		pegtl::seq<
			expression,
			pegtl::one< ',' >,
			expression_list
		>,
		expression
	>
	{};
	
	struct assignment : pegtl::seq<
		identifier,
		pegtl::one< '=' >,
		expression
	>
	{};
	
	struct statement : pegtl::sor<
		pegtl::seq<
			function,
			pegtl::opt<pegtl::eol>
		>,
		pegtl::seq<
			assignment,
			pegtl::eol
		>
	>
	{};
	
	struct statement_list;	
	// function()\nfunction()...
	struct statement_list : pegtl::sor<
		pegtl::seq<
			statement,
			statement_list
		>,
		statement
	>
	{};
	
	struct program : statement_list
	{};
	
	// Rules
	template <typename Rule>
	struct action : pegtl::nothing< Rule > {};
	
	// Specialize to capture numbers
	template <>
	struct action<number> 
	{
		template <typename ActionInput>
		static void apply(const ActionInput& in, std::string& s){
			s += in.string() + " ";
			std::cout << in.string() << std::endl;
		}	
	};
	
	// Selector
	template< typename Rule >
	using selector = tao::pegtl::parse_tree::selector< Rule,
		tao::pegtl::parse_tree::store_content::on<
			number,
			identifier,
			function,
			expression,
			expression_list,
			statement,
			statement_list,
			program,
			assignment,
			op_prio_add,
			op_prio_mul
		>,
		tao::pegtl::parse_tree::fold_one::on<
			add_a,
			mul_a
		>
	>;
	
	// Execution
	
	
	
} //worldlang
