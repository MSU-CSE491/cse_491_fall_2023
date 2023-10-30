#pragma once

// Debug
#include <iostream>
#include <functional>

#include <map>
#include <stack>
#include <variant>

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
	
	//[_a-zA-Z][_a-zA-Z0-9]*
	// (C-style identifier, case sensitive)
	struct identifier : pegtl::identifier
	{};
	
	// identifier list
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
		identifier_list,
		pegtl::one< '=' >,
		expression_list
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
			function,
			identifier,
			identifier_list,
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
			} else if (type == "worldlang::function"){
				// arg arg arg ... argument_count function_name
				if (node->children.size() > 1)
					traverse(node->children[1]);
				// TODO: identify argument list size...
				// expression_list or expression child
				int argcount = 0;
				if (node->children.size() == 1){
					argcount = 0;
				} else {
					argcount = count(node->children.at(1));
				}
				
				out.push_back(Unit{Unit::Type::number, std::to_string(argcount)});
				out.push_back(Unit{Unit::Type::function, node->children.at(0)->string()});
			} else if (type == "worldlang::assignment"){
				// identifier_list
				auto identifier_count = count(node->children.at(0));
				traverse(node->children.at(0));
				// value
				// Can't check expression count here: must be done at runtime
				// since functions may have multiple returns
				traverse(node->children.at(1));
				// expression
				out.push_back(Unit{Unit::Type::number, std::to_string(identifier_count)});
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
		} else {
			// parse error lol
			std::cout << "Parse error!!" << std::endl;
		}
			
		return out;
	}
	
//	class ProgramExecutor;
	
	class ProgramExecutor {
	// Internal types
	public:
		using Callable = std::function<void(ProgramExecutor&)>;
		
		struct Identifier : std::string {}; 
		
		using Value = std::variant < double, std::string, Callable, Identifier >;
		
	// Execution state
	private:
		/// Variables
		std::map < std::string, Value > variables;
		
		/// Execution stack
		std::stack < Value > stack;
		
		/// Error message
		std::string error_message{};
		
	// Public methods
	public:
		/// Constructor
		ProgramExecutor() = default;
		
		void registerFunction(std::string name, Callable callable){
			variables.insert_or_assign(name, callable);
		}
		
		Value popStack(){
			auto v = stack.top();
			stack.pop();
			return v;
		}
		
		void pushStack(Value v){
			stack.push(v);
		}
		
		// check whether or not this Value contains the expected type
		template <typename T>
		bool has(const Value& a){
			if (std::holds_alternative<T>(a)){
				return true;
			} else if (std::holds_alternative<Identifier>(a)){
				try {
					auto val = variables.at(static_cast<std::string>(std::get<Identifier>(a)));
					return std::holds_alternative<T>(val);
				} catch(const std::out_of_range& e) {
					error("Variable does not exist!");
				}
			}
			return false;
		}
		
		// TODO figure out what kind of doc comments we're actually using
		/// Get a value of Value, whether it contains a value or an identifier
		/// that contains that value
		/// 
		/// Example:
		/// If your program consists of a=5\nb=a\n
		/// then as<double> will handle both 5 and a correctly as arguments
		/// std::get<double> is longer and only handles 5.
		template <typename T>
		T as(const Value& a){
			if (std::holds_alternative<T>(a)){
				return std::get<T>(a);
			} else if (std::holds_alternative<Identifier>(a)){
				auto val = variables.at(static_cast<std::string>(std::get<Identifier>(a)));
				if (std::holds_alternative<T>(val)){
					return std::get<T>(val);
				}
			}
			// error if conversion fails
			error("Type error in as()!");
			return T{};
		}
		
		// Gets the value of a variable
		// Throws std::out_of_range if it is not defined
		// Throws std::bad_variant_access if variable is wrong type
		template <typename T>
		T var(const std::string& name){
			auto val = variables.at(name);
			return std::get<T>(val);
		}
		
		void error(const std::string& error){
			if (error_message.empty()){
				error_message = error;
			}
		}
		
		std::string getErrorMessage(){
			return error_message;
		}
		
		/// Returns false if program had an error
		bool run(const std::string& program){
			//TODO: program preprocessing (add newline to end, remove spaces)
			error_message = "";
			auto code = parse_to_code(program);
			//TODO: check for parse success
			
			size_t index = 0;
			while (error_message.empty() && index < code.size()){
				auto& unit = code.at(index);
				switch (unit.type){
					case Unit::Type::number:
						std::cout << "Push number " << unit.value << std::endl;
						try {
							pushStack(std::stod(unit.value));
						} catch (const std::invalid_argument& e) {
							error("Failed to convert number!");
						} catch (const std::out_of_range& e){
							error("Number too big!");
						}
						break;
					
					case Unit::Type::identifier:
						std::cout << "Push identifier " << unit.value << std::endl;
						pushStack(Identifier{unit.value});
						break;
					
					case Unit::Type::operation:
						// perform operation!
						std::cout << "Perform operation " << unit.value << std::endl;
						if (std::string{"+-*/"}.find(unit.value) != std::string::npos){
							// binary expressions
							auto b = popStack();
							auto a = popStack();
							if (unit.value == "+"){
								if (has<double>(a) && has<double>(b)){
									pushStack(as<double>(a) + as<double>(b));
								} else if (has<std::string>(a) && has<std::string>(b)){
									pushStack(as<std::string>(a) + as<std::string>(b));
								} else {
									error("Runtime type error (plus)");
								}
							} else if (unit.value == "-"){
								if (has<double>(a) && has<double>(b)){
									pushStack(as<double>(a) - as<double>(b));
								} else {
									error("Runtime type error (minus)");
								}
							} else if (unit.value == "*"){
								if (has<double>(a) && has<double>(b)){
									pushStack(as<double>(a) * as<double>(b));
								} else if (has<std::string>(a) && has<double>(b)){
									std::string n;
									double c = as<double>(b);
									for (int i = 0; i < c; ++i){
										n += as<std::string>(a);
									}
									pushStack(n);
								} else {
									error("Runtime type error (times)");
								}
							} else if (unit.value == "/"){
								if (has<double>(a) && has<double>(b)){
									pushStack(as<double>(a) / as<double>(b));
								} else {
									error("Runtime type error (divide)");
								}
							}
						} else if (unit.value == "="){
							// number of arguments
							auto count = std::get<double>(popStack());
							// values to assign
							std::vector< Value > values;
							// identifiers to assign to
							std::vector< Identifier > identifiers;
							//ex. a,b,c=1,2,3
							// v: 3 2 1
							// i: c b a
							std::cout << "Assignment of #" << count << std::endl;
							
							for (int i = 0; i < count; ++i){
								values.push_back(popStack());
							}
							for (int i = 0; i < count; ++i){
								auto v = popStack();
								if (std::holds_alternative<Identifier>(v)){
									identifiers.push_back(std::get<Identifier>(v));
								} else {
									error("Tried to assign to non-identifier type");
									goto break_interpreter_loop;
								}
							}
							
							// upon reaching this point, values and identifiers
							// are the same length and contain valid items.
							for (int i = 0; i < count; ++i){
								auto a = identifiers[i];
								auto b = values[i];
								
								if (!std::holds_alternative<Identifier>(b)){
									variables.insert_or_assign(static_cast<std::string>(a), b);
								} else {
									// exception if variable b does not exist
									try {
										auto& b_var = variables.at(static_cast<std::string>(std::get<Identifier>(b)));
										variables.insert_or_assign(static_cast<std::string>(a), b_var);
									} catch (const std::out_of_range& e){
										error("Variable did not exist!");
									}
								}
							}
						} else if (unit.value == "endline"){
							// clear stack on end of line
							while (stack.size())
								popStack();
						} else {
							error("Unknown operation '" + unit.value + "'");
						}
						break;
					
					case Unit::Type::function:
						std::cout << "Perform function " << unit.value << std::endl;
						if (variables.count(unit.value)){
							auto& func = variables.at(unit.value);
							if (std::holds_alternative<Callable>(func)){
								std::get<Callable>(func)(*this);
							} else {
								error("Not a callable object!");
							}
							break;
						} else {
							error("Function does not exist!");
						}
						break;
					
					default:
						error("Unknown code unit '" + unit.value +"'!");
				}
				index++;
			}
			break_interpreter_loop:
			std::cout << "Program execution ends" << std::endl;
			if (!error_message.empty()){
				std::cout << "With error: " << error_message << std::endl;
			}
			
			return error_message.empty();
		}
	};
} //worldlang
