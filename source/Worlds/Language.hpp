#pragma once

// Debug
#include <iostream>
#include <functional>

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
	
	struct Unit {
		enum class Type {
			number,
			identifier,
			function,
			operation,
		} type;
		
		// todo: make variant?
		std::string value;
	};
	
	// Parsing final form
	std::vector<Unit> parse_to_code(std::string program){
		pegtl::string_input in(program, "program");
		std::vector<Unit> out{};
		
		std::function<void(const std::unique_ptr<pegtl::parse_tree::node>&)> traverse =
		[&out, &traverse](const std::unique_ptr<pegtl::parse_tree::node>& node) -> void{
			const std::string_view& type = node->type;
			// visit:
			if (type == "worldlang::number"){
				out.push_back(Unit{Unit::Type::number, node->string()});
			} else if (type == "worldlang::identifier"){
				out.push_back(Unit{Unit::Type::identifier, node->string()});
			} else if (type == "worldlang::function"){
				// arg arg arg ... argument_count function_name
				traverse(node->children[1]);
				// TODO: identify argument list size...
				
				out.push_back(Unit{Unit::Type::function, node->children[0]->string()});
			} else if (type == "worldlang::assignment"){
				// identifier
				traverse(node->children.at(0));
				// value
				traverse(node->children.at(1));
				// expression
				out.push_back(Unit{Unit::Type::operation, "="});
			} else if (type == "worldlang::mul_a"
					|| type == "worldlang::add_a"
					|| type == "worldlang::expression"){
				traverse(node->children.at(0));
				if (node->children.size() > 1){
					traverse(node->children.at(2));
					out.push_back(Unit{Unit::Type::operation, node->children[1]->string()});
				}
			} else if (type == "worldlang::expression_list"){
				if (node->children.size()){
					for (const auto& c : node->children){
						traverse(c);
					}
				}
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
		} else {
			// parse error lol
			std::cout << "lolmao" << std::endl;
		}
			
		return out;
	}
	
	
} //worldlang
