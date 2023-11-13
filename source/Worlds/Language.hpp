#pragma once

// Debug
#include <iostream>

#include <functional>

#include <tao/pegtl.hpp>
#include <tao/pegtl/contrib/parse_tree.hpp>

namespace pegtl = tao::pegtl;

namespace worldlang{
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
	
	struct string : pegtl::seq<
		pegtl::one< '"' >,
		pegtl::star<
			pegtl::not_one<'"'>
		>,
		pegtl::one< '"' >
	>
	{};
	
	//[_a-zA-Z][_a-zA-Z0-9]*
	// (C-style identifier, case sensitive)
	struct identifier : pegtl::identifier
	{};
	
	// identifier list
	// Contains one or more identifiers.
	// This matches expressions such as
	// "name" or "a,b,c".
	struct identifier_list;	
	struct identifier_list : pegtl::sor<
		pegtl::seq<
			identifier,
			pegtl::one< ',' >,
			identifier_list
		>,
		identifier
	>
	{};
	
	// Match operators of same priority as addition
	struct op_prio_add : pegtl::one< '+', '-' >
	{};
	
	// Match operators of same priority as multiplication
	struct op_prio_mul : pegtl::one< '*', '/' >
	{};
	
	// Must forward-declare for recursion
	struct expression;
	struct expression_list;	
	
	// Function call
	// Matches expressions of form
	// "id(arg1,arg2,...)" or "id()"
	struct function : pegtl::seq<
		identifier,
		pegtl::one< '(' >,
		pegtl::opt<expression_list>,
		pegtl::one< ')' >
	>
	{};
	
	// Match an expression or element representing a single value
	// examples: 5,(-38),myvar1,myfunc1(7)
	// Note that functions can technically return multiple values or none,
	// so this isn't perfect.
	// Expressions within parentheses is also counted as an element to allow
	// for recursion, so "(3+5+8)" is a valid element
	struct element : pegtl::sor<
		function,
		identifier,
		number,
		string,
		pegtl::seq<
			pegtl::one< '(' >,
			expression,
			pegtl::one< ')' >
		>
	>
	{};
	
	// Matches one multiplication expression or a single element
	// "a*b" or "c"
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
	// Recursively matches multiplication operations
	// Ex. "1*4", "a*b*c"
	struct mul : pegtl::sor<
		pegtl::seq<
			mul_a,
			op_prio_mul,
			mul
		>,
		mul_a
	>
	{};
	
	// Matches one addition expression or a multiplication subexpression
	// "a+b" or "a+b*c", with "b*c" as the subexpression
	struct add_a : pegtl::sor<
		pegtl::seq<
			mul,
			op_prio_add,
			mul
		>,
		mul
	>
	{};	
	
	// Matches entire addition expression
	// Because this is currently the top level of expression, it matches any
	// expression as well.
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
	
	// Match an expression intended to evaluate to a single value.
	struct expression : pegtl::sor<
		add	
	>
	{};
	
	struct expression_list;
	// A list of expressions. Can contain one or more expressions (not zero!)
	// "123,345+456,func(3)+6*8"
	struct expression_list : pegtl::sor<
		pegtl::seq<
			expression,
			pegtl::one< ',' >,
			expression_list
		>,
		expression
	>
	{};
	
	// Assignment expression
	// "var=value", or "var,vars=val1,val2", etc.
	struct assignment : pegtl::seq<
		identifier_list,
		pegtl::one< '=' >,
		expression_list
	>
	{};
	
	struct statement_list;
	
	// Matches a block of code
	// ex. { /*code*/ }
	struct code_block : pegtl::seq<
		pegtl::one< '{' >,
		pegtl::eol,
		statement_list,
		pegtl::one< '}' >,
		pegtl::eol
	>
	{};
	
	struct statement : pegtl::sor<
		pegtl::seq<
			function,
			pegtl::opt<code_block>,
			pegtl::opt<pegtl::eol>
		>,
		pegtl::seq<
			assignment,
			pegtl::opt<pegtl::eol>
		>,
		pegtl::eol
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
			string,
			function,
			identifier,
			identifier_list,
			expression,
			expression_list,
			statement,
			statement_list,
			code_block,
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
	
	struct Unit {
		enum class Type {
			number,
			string,
			identifier,
			function,
			operation,
		} type;
		
		// todo: make variant?
		std::string value;
	};
	
	
	/**
	 * Converts a program string into code units using PEGTL
	 */
	std::vector<Unit> parse_to_code(std::string program){
		pegtl::string_input in(program, "program");
		std::vector<Unit> out{};
		
		// recursive "count arugments" function
		std::function<int(const std::unique_ptr<pegtl::parse_tree::node>&)> count =
		[&count](const std::unique_ptr<pegtl::parse_tree::node>& node) -> int{
			if (node->children.size() > 1)
				return 1 + count(node->children[1]);
			return 1;
		};
		
		std::function<void(const std::unique_ptr<pegtl::parse_tree::node>&)> traverse =
		[&out, &traverse, &count](const std::unique_ptr<pegtl::parse_tree::node>& node) -> void{
			const std::string_view& type = node->type;
			// visit:
			if (type == "worldlang::number"){
				out.push_back(Unit{Unit::Type::number, node->string()});
			} else if (type == "worldlang::identifier"){
				out.push_back(Unit{Unit::Type::identifier, node->string()});
			} else if (type == "worldlang::string"){
				// trim quotes off
				out.push_back(Unit{Unit::Type::string, node->string().substr(1,node->string().size()-2)});
			} else if (type == "worldlang::function"){
				// (operator_endargs) arg arg arg function_name
				out.push_back(Unit{Unit::Type::operation, "endargs"});
				if (node->children.size() > 1)
					traverse(node->children[1]);
				
//				out.push_back(Unit{Unit::Type::number, std::to_string(argcount)});
				out.push_back(Unit{Unit::Type::function, node->children.at(0)->string()});
			} else if (type == "worldlang::assignment"){
				// identifier_list
//				auto identifier_count = count(node->children.at(0));
				out.push_back(Unit{Unit::Type::operation, "endargs"});
				traverse(node->children.at(0));
				// value
				// Can't check expression count here: must be done at runtime
				// since functions may have multiple returns
				out.push_back(Unit{Unit::Type::operation, "endargs"});
				traverse(node->children.at(1));
				// expression
//				out.push_back(Unit{Unit::Type::number, std::to_string(identifier_count)});
				out.push_back(Unit{Unit::Type::operation, "="});
			} else if (type == "worldlang::mul_a"
					|| type == "worldlang::add_a"
					|| type == "worldlang::expression"){
				traverse(node->children.at(0));
				if (node->children.size() > 1){
					traverse(node->children.at(2));
					out.push_back(Unit{Unit::Type::operation, node->children.at(1)->string()});
				}
			} else if (type == "worldlang::expression_list"
					|| type == "worldlang::identifier_list"){
				std::cout << "Traversing " << type << "\n";
				if (node->children.size()){
					for (const auto& c : node->children){
						traverse(c);
					}
				}
			} else if (type == "worldlang::code_block"){
				std::cout << "Traversing " << type << "\n";
				out.push_back(Unit{Unit::Type::operation, "start_block"});
				traverse(node->children.at(0));
				out.push_back(Unit{Unit::Type::operation, "end_block"});
			} else if (type == "worldlang::statement"){
				for (const auto& child : node->children){
					traverse(child);
				}
				out.push_back(Unit{Unit::Type::operation, "endline"});
			} else {
				std::cout << "Type: " << type;
				if (node->has_content())
					std::cout << " Content: " << node->string();
				std::cout << std::endl;
				// visit all children
				for (const auto& child : node->children){
					traverse(child);
				}
			}
		};
		
		auto root = pegtl::parse_tree::parse < worldlang::program, worldlang::selector > (in);
		
		if (root){
			traverse(root->children[0]);
    		return out;
		} else {
			// parse error lol
			std::cout << "Parse error!!" << std::endl;
			return {};
		}
	}
	
} //worldlang
