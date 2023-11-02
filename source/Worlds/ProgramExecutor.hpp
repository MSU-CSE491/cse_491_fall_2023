#pragma once

#include "Language.hpp"

#include "core/WorldBase.hpp"

#include <algorithm>
#include <functional>
#include <map>
#include <stack>
#include <variant>

namespace worldlang {
	class ProgramExecutor {
	// Internal types
	public:
		using Callable = std::function<void(ProgramExecutor&)>;
		
		struct Identifier : std::string {}; 
		
		using Value = std::variant < double, std::string, Callable, Identifier >;
		
	// Execution state
	private:
		/// Variables
		std::map < std::string, Value > variables{};
		
		/// Execution stack
		std::stack < Value > stack{};
		
		/// Error message
		std::string error_message{};
		
	// Public methods
	public:
		/// Constructor
		ProgramExecutor() = default;
		
		/// Constructor with function registration
		ProgramExecutor(cse491::WorldBase& world) : ProgramExecutor(){
			// Load world grid from file
			registerFunction("loadWorld", [this, &world](ProgramExecutor& pe){
				auto args = pe.popArgs();
				if (args.size() != 1) { error("Wrong number of arguments!"); return; }
				world.GetGrid().Read(as<std::string>(args.at(0)), world.GetCellTypes());
			});
			// Get the size of the world
			registerFunction("getWorldSize", [this, &world](ProgramExecutor& pe){
				auto args = pe.popArgs();
				if (args.size() != 0) { error("Wrong number of arguments!"); return; }
				pe.pushStack(static_cast<double>(world.GetGrid().GetWidth()));
				pe.pushStack(static_cast<double>(world.GetGrid().GetHeight()));
			});
		}
		
		void registerFunction(std::string name, Callable callable){
			variables.insert_or_assign(name, callable);
		}
		
		Value popStack(){
			auto v = stack.top();
			stack.pop();
			return v;
		}
		
		std::vector<Value> popArgs(){
			std::vector< Value > values;
			do {
				values.push_back(popStack());
			} while (!(has<Identifier>(values.back()) 
					&& static_cast<std::string>(as<Identifier>(values.back())) == "__INTERNAL_ENDARGS"));
			values.pop_back(); // don't keep that one
			std::ranges::reverse(values);
			return values;
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
			error(std::string{"Type error in as()! Expected "}+typeid(T).name());
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
		
		bool runFile(const std::string& filename){
			std::ifstream in{filename};
			std::string s;
			std::string filedata;
			while (getline(in, s))
				filedata += s + '\n';
			// probably works idk
			std::cout << "Program from file: " << filedata << std::endl;
			return run(filedata);
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
					
					case Unit::Type::string:
						std::cout << "Push string " << unit.value << std::endl;
						pushStack(unit.value);
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
							// values to assign
							std::vector< Value > values = popArgs();
							// identifiers to assign to
							auto identifier_values = popArgs();
							if (values.size() > identifier_values.size()){
								error("Too many values!");
								break;
							} else if (values.size() < identifier_values.size()){
								error("Not enough values!");
								break;
							}
							// Convert to identifiers specifically
							std::vector< Identifier > identifiers;
							std::ranges::transform(identifier_values, std::back_inserter(identifiers), [this](const Value& v){ return as<Identifier>(v); });
							//ex. a,b,c=1,2,3 becomes the follwoing units
							// . a b c . 1 2 3 =
							// v: 3 2 1
							// i: c b a
							if (!getErrorMessage().empty()){
								// transform failed in some way 
								break;
							}
							
							// upon reaching this point, values and identifiers
							// are the same length and contain valid items.
							for (size_t i = 0; i < identifiers.size(); ++i){
								auto a = identifiers[i];
								auto b = values[i];
								
								if (!std::holds_alternative<Identifier>(b)){
									variables.insert_or_assign(static_cast<std::string>(a), b);
								} else {
									// exception if variable b does not exist
									try {
										auto& b_var = variables.at(static_cast<std::string>(as<Identifier>(b)));
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
						} else if (unit.value == "endargs"){
							// this could absolutely be broken but that's OK
							pushStack(Identifier{"__INTERNAL_ENDARGS"});
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
								error(unit.value + " is not a callable object!");
							}
							break;
						} else {
							error("Function " + unit.value + " does not exist!");
						}
						break;
					
					default:
						error("Unknown code unit '" + unit.value +"'!");
				}
				index++;
			}
			
			std::cout << "Program execution ends" << std::endl;
			if (!error_message.empty()){
				std::cout << "With error: " << error_message << std::endl;
			}
			
			return error_message.empty();
		}
	};
} //worldlang
