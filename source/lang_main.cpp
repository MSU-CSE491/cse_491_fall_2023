/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief A simplistic main file to demonstrate the language system.
 * @note Status:TESTING
 **/

// Include the modules that we will be using.
#include "Worlds/Language.hpp"

int main()
{
	pegtl::memory_input in("test + wow", "");

	pegtl::parse< worldlang::expression > (in);
}
