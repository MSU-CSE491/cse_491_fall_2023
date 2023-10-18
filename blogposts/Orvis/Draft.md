# ChatGPT Calculator

by Evan Orvis

## Background

ChatGPT is a very powerful large language model-based chatbot that can be used in a variety of situations. Computer Scientists are able to use this technology in a variety of ways to help write simple programs. While AI is generally pretty good at mathematical functions, chatGPT still has some areas that it struggles in. 

## Simple Code Generation

```cpp
#include <iostream>

int main() {
	double num1, num2;
	char operation;

	std::cout << "Enter the first number: ";
	std::cin >> num1;

	std::cout << "Enter an operation (+, -, *, /): ";
	std::cin >> operation;

	std::cout << "Enter the second number: ";
	std::cin >> num2;

	double result;

	switch (operation) {
		case '+':
			result = num1 + num2;
			break;
		case '-':
			result = num1 - num2;
			break;
		case '*':
			result = num1 * num2;
			break;
		case '/':
			if (num2 != 0) {
				result = num1 / num2;
			} else {
				std::cout << "Error: Division by zero is not allowed." << std::endl;
				return 1; // Exit with an error code
			}
			break;
		default:
			std::cout << "Invalid operation. Please use +, -, *, or /." << std::endl;
			return 1; // Exit with an error code
	}

	std::cout << "Result: " << num1 << " " << operation << " " << num2 << " = " << result << std::endl;

return 0;
}
```

For this simple code, all that was asked was for chatGPT to write some c++ code that can do four simple arithmetic functions: addition, subtraction, multiplication, and division. In this regard, it creates quality code that is very efficient. For this simple code generation, I was a little surprised that chatGPT used a switch statement, as it is a slightly more advanced and efficient method to handle operations. I was also surprised that chatGPT knew to account for dividing by zero, as it is something that people can often overlook. ChatGPT overall created very good code that can do simple arithmatic operations.

## Adding more features to the code

```cpp
temp
```

## 
