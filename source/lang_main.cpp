/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief A simplistic main file to demonstrate the language system.
 * @note Status:TESTING
 **/

// Include the modules that we will be using.
#include "Worlds/Language.hpp"
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
//	pegtl::memory_input in("2+test(123*5+4-6/7,abc)-wow", "src");
	pegtl::memory_input in("var_1=2+test(123*5+4-6/7,abc)-wow\ntest(123,435,7650+8*8)\n", "src");
	std::string s;
	// pegtl::parse< worldlang::expression, worldlang::action > (in, s);
	auto root = pegtl::parse_tree::parse < worldlang::program, worldlang::selector > (in);
	
	if (root){
		traverse(root);
	} else {
		std::cout << "Failed to match grammar!" << std::endl;
	}
}
