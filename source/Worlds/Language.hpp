/// @brief File defining language syntax and parsing.
#pragma once

// Debug
#include <iostream>

#include <functional>

#include <tao/pegtl.hpp>
#include <tao/pegtl/contrib/parse_tree.hpp>

#include "core/EasyLogging.hpp"

using clogged::Logger;
using clogged::Team;
using clogged::LogLevel;

namespace pegtl = tao::pegtl;

/// Namespace for scripting language stuff
namespace worldlang{
	/**
	 * function to strip whitespace
	*/
	std::string stripWhitespace(const std::string& input) {
        std::string result;
        bool insideString = false;

        for (char c : input) {
            if (c == '\"') {
                insideString = !insideString;
            }
			// https://en.cppreference.com/w/cpp/string/byte/isspace
            if (!insideString && std::isspace(c) && c != '\n') {
                // Skip whitespace outside of strings, excluding newlines
                continue;
            }
            result += c;
        }
        return result;
    }
	
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
	
	// Match operators of same priority as comparison ops
	struct op_prio_comp : pegtl::sor< 
		TAO_PEGTL_STRING("=="),
		TAO_PEGTL_STRING("!="),
		TAO_PEGTL_STRING("<="),
		TAO_PEGTL_STRING(">="),
		pegtl::one< '<' >,
		pegtl::one< '>' >
	>
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
	
	struct comp_a : pegtl::sor<
		pegtl::seq<
			add,
			op_prio_comp,
			add
		>,
		add
	>
	{};	
	
	// Matches entire comparison expression
	struct comp;
	struct comp : pegtl::sor<
		pegtl::seq<
			comp_a,
			op_prio_comp,
			comp
		>,
		comp_a
	>
	{};
	
	// Match an expression intended to evaluate to a single value.
	struct expression : pegtl::sor<
		comp
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
	
	// Comment
	// # Comment text goes here
	struct comment : pegtl::seq<
		pegtl::one< '#' >,
		pegtl::until<
			pegtl::eol
		>
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
		comment,
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
	
	struct program : pegtl::seq<
		statement_list,
		pegtl::eolf
	>
	{};
	
	/// Selector for tree generation
	/// This is used by PEGTL's parse_tree function to determine
	/// which nodes are kept, which are folded into their parent when possible,
	/// and which are ignored (these are not listed here).
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
			op_prio_mul,
			op_prio_comp
		>,
		tao::pegtl::parse_tree::fold_one::on<
			add_a,
			mul_a
		>
	>;
	
	/// A singular code unit.
	/// Will have some specific meaning within the interpreter itself.
	struct Unit {
		/// Various types of code Units.
		/// @note that most internal interpreter functions all fall under `operation`.
		enum class Type {
			number,
			string,
			identifier,
			function,
			operation,
			function_decl,
		};
		/// This determines how this unit is used within the interpreter.
		/// The type of this code unit.
		Type type;
		
		/// The value of this code unit. What it means depends on the type.
		std::string value;
	};
	
	
	/**
	 * Converts a program string into code units using PEGTL
	 */
	std::vector<Unit> parse_to_code(std::string program){
		program = stripWhitespace(program);
		pegtl::string_input in(program, "program");
		std::vector<Unit> out{};
		
		auto log = Logger::Log() << Team::TEAM_4 << LogLevel::DEBUG;
		log << "Entering parser" << std::endl;
		
		std::function<void(const std::unique_ptr<pegtl::parse_tree::node>&)> traverse =
		[&out, &traverse, &log](const std::unique_ptr<pegtl::parse_tree::node>& node) -> void{
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
				log << "Traversing " << type << "\n";
				if (node->children.size()){
					for (const auto& c : node->children){
						traverse(c);
					}
				}
			} else if (type == "worldlang::code_block"){
				log << "Traversing " << type << "\n";
				out.push_back(Unit{Unit::Type::operation, "start_block"});
				traverse(node->children.at(0));
				out.push_back(Unit{Unit::Type::operation, "end_block"});
			} else if (type == "worldlang::statement"){
				if (node->children.size()){
//					auto value = node->children.at(0)->string();
					auto& first = node->children.at(0);
					if (first->type == "worldlang::function" 
						&& first->children.at(0)->string() != "if"
						&& first->children.at(0)->string() != "for"
						&& node->children.size() > 1
						&& node->children.at(1)->type == "worldlang::code_block"){
						// encode differently here to use for definitions
						// <values> . a b c d e funcname(decl)
						// funcname assigns variables, then jumps to code
						traverse(node->children.at(0)); // function
						out.back().type = Unit::Type::function_decl;
						traverse(node->children.at(1)); // function code
					} else {
						for (const auto& child : node->children){
							traverse(child);
						}
					}
				}
				out.push_back(Unit{Unit::Type::operation, "endline"});
			} else {
				log << "Type: " << type;
				if (node->has_content())
					log << " Content: " << node->string();
				log << std::endl;
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
			log << LogLevel::WARNING << "Parse error!!" << std::endl;
			return {};
		}
	}
	
} //worldlang
