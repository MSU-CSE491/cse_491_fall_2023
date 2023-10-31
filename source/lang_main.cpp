/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief A simplistic main file to demonstrate the language system.
 * @note Status:TESTING
 **/

// Include the modules that we will be using.
#include "Worlds/Language.hpp"
#include "Worlds/ProgramExecutor.hpp"
// Used to build the parse tree
#include <tao/pegtl/contrib/parse_tree.hpp>

void traverse(const auto& node, int indent = 0){
	std::cout << std::string(indent, ' ');
	std::cout << "Node: " << node->type;
	if (node->has_content())
		std::cout << " Text: " << node->string_view();
	std::cout << " Childs: " << node->children.size() << "\n";
	for (const auto& c : node->children){
		traverse(c, indent+4);
	}
}

int main()
{
/*
//	pegtl::memory_input in("a=2+test(123*5+4-6/7,abc)-wow\n", "src");
//	pegtl::memory_input in("var_1=2+test(123*5+4-6/7,abc)-wow\ntest(123,435,7650+8*8)\n", "src");
	std::string s;
	// pegtl::parse< worldlang::expression, worldlang::action > (in, s);
	auto root = pegtl::parse_tree::parse < worldlang::program, worldlang::selector > (in);
	
	if (root){
		traverse(root);
	} else {
		std::cout << "Failed to match grammar!" << std::endl;
	}
*/
	std::string program = "test()\ntest(1)\nwow=3\na=2+test(123*5+4-6/7,abc)-wow\ntest(123-a,435,7650+8*8)\n";
	
	auto code = worldlang::parse_to_code(program);
	for (auto& c : code){
		std::cout << static_cast<int>(c.type) << ", " << c.value << std::endl;
	}
	
	using worldlang::ProgramExecutor;
	auto test = [](ProgramExecutor& pe){
		//TODO: catch exception here? should never be needed?
		auto args = pe.popArgs();
		auto count = args.size();
		std::cout << "Called test with " << count << " args (note: order reversed here):\n";
		for (int i = 0; i < count; ++i){
			auto arg = args[0];
			if (std::holds_alternative<double>(arg)){
				std::cout << std::get<double>(arg);	
			} else if (std::holds_alternative<std::string>(arg)){
				std::cout << std::get<std::string>(arg);
			} else if (std::holds_alternative<ProgramExecutor::Identifier>(arg)){
				std::cout << std::get<ProgramExecutor::Identifier>(arg);
			}
			if (i != count - 1){
				std::cout << ", ";
			} else {
				std::cout << std::endl;
			}
		}
		// result value
		pe.pushStack(0.0);
	};
	
	worldlang::ProgramExecutor pe;
	pe.registerFunction("test", test);
	pe.run(program);
}

