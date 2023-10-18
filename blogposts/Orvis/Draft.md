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

Since the simle code generation was practically perfect, the level of difficulty needed to be increased. This was done by asking chatGPT to add an ability to compute another operation and add operations for exponential, square root, and modulo operations.

```cpp
#include <iostream>
#include <cmath> // For square root and exponentiation

int main() {
    char repeat;
    do {
        double num1, num2;
        char operation;

        std::cout << "Enter the first number: ";
        std::cin >> num1;

        std::cout << "Enter an operation (+, -, *, /, ^ (exponentiation), sqrt (square root), % (modulo)): ";
        std::cin >> operation;

        if (operation != 'sqrt' && operation != '^' && operation != '%') {
            std::cout << "Enter the second number: ";
            std::cin >> num2;
        }

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
                    continue; // Repeat the loop
                }
                break;
            case '^':
                result = std::pow(num1, num2);
                break;
            case 'sqrt':
                result = std::sqrt(num1);
                break;
            case '%':
                result = std::fmod(num1, num2);
                break;
            default:
                std::cout << "Invalid operation. Please use +, -, *, /, ^, sqrt, or %." << std::endl;
                continue; // Repeat the loop
        }

        std::cout << "Result: ";
        if (operation == 'sqrt') {
            std::cout << "sqrt(" << num1 << ") = " << result;
        } else {
            std::cout << num1 << " " << operation << " " << num2 << " = " << result;
        }
        std::cout << std::endl;

        std::cout << "Do you want to perform another operation? (y/n): ";
        std::cin >> repeat;
    } while (repeat == 'y' || repeat == 'Y');

    return 0;
}
```

This code is more complex and starts to show where chatGPT struggles. The first problem that appeared was that the operation is being inputted as a char and chatGPT set 'sqrt' as the char for square root operation, which is impossible to do. When pointed out, chatGPT agreed and changed the operation to 's'. Another big error was that chatGPT was excluding exponential and modulo operations from entering a second number, which is needed for both operations. These mistakes were both part of adding multiple operations where is looks like chatGPT took the mathematical properties from the first operation and transposed them onto the others as well. It is impressive that chatGPT includes the cmath library to be able to use simpler functions for operations. Overall, it is a little surprising that chatGPT struggled with this code.

## Adding even more features

For the next section, I already knew that chatGPT had failed to write complete code and I started to see if more simple and complex operations would break anything. For this, operations for absolute value, inverse, log, and natural log, as well as the ability to reapeat operations with the result from the previous computation

```cpp
temp
```

stuff here

## extra

```cpp
temp
```

stuff here


