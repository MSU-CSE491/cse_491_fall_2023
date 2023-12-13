# ChatGPT Calculator

*by Evan Orvis*

## Background

ChatGPT is a very powerful large language model-based chatbot that can be used in a variety of situations. Computer Scientists are able to use this technology in a variety of ways to help write simple programs. While AI is generally pretty good at mathematical functions, ChatGPT still has some areas that it struggles in. ChatGPT can create some quality and efficient code, but when multiple things are added at a single time, it can easily forget how to correctly add it all into the program.

## Simple Code Generation

The initial prompt for the simple code generation was: "write me c++ code that can do addition, subtraction, multiplication, and division".

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

For this simple code, all that was asked for ChatGPT to write some C++ code that can do four simple arithmetic functions: addition, subtraction, multiplication, and division. In this regard, it creates quality code that is very efficient. For this simple code generation, it was a little surprising that ChatGPT used a switch statement, as it is a slightly more advanced and efficient method to handle operations. It was also surprising that ChatGPT knew to account for dividing by zero, as it is something that people can often overlook. ChatGPT overall created very good code that can do simple arithmetic operations.

## Adding more features to the code

Since the simple code generation was practically perfect, the level of difficulty needed to be increased. This was done by asking ChatGPT to add an ability to compute another operation and add operations for exponential, square root, and modulo operations.

The prompt for this section was the following: "for the previous code, allow the user to repeat operations" and "Also allow exponential, square root, and modulo function as valid operation".

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

This code is more complex and starts to show where ChatGPT struggles. The first problem that appeared was that the operation is being inputted as a char and ChatGPT set 'sqrt' as the char for square root operation, which is impossible to do. When pointed out, ChatGPT agreed and changed the operation to 's'. Another big error was that ChatGPT was excluding exponential and modulo operations from entering a second number, which is needed for both operations. These mistakes were both part of adding multiple operations where it looks like ChatGPT took the mathematical properties from the first operation and transposed them onto the others as well. However, it is impressive that ChatGPT includes the cmath library to be able to use simpler functions for operations. Overall, it is a little surprising that ChatGPT struggled with this code.

To fix these errors, the following prompts were used: "sqrt is not a valid char" and "exponentiation and modulo need a second number".

## Adding even more features

For the next section, since ChatGPT had already failed to write complete code, it makes sense to see if more operations would break anything. For this, operations for absolute value, inverse, log, and natural log, as well as the ability to repeat operations with the result from the previous computation.

This added a lot of functionality but took a few prompts to get it all in: "add an inverse and absolute value operation", "also add log and ln operations", and "have the program automatically repeat with the result becoming the first number".

```cpp
#include <iostream>
#include <cmath> // For mathematical functions

int main() {
    char repeat;
    double result; // Variable to store the result

    do {
        double num1, num2;
        char operation;

        // If there is a previous result, use it as the new first number
        if (result != 0.0) {
            num1 = result;
        }

        std::cout << "Enter an operation (+, -, *, /, ^ (exponentiation), s (square root), % (modulo), i (inverse), a (absolute value), l (log), n (ln)): ";
        std::cin >> operation;

        if (operation != 's' && operation != 'i' && operation != 'a' && operation != 'l' && operation != 'n') {
            std::cout << "Enter the second number: ";
            std::cin >> num2;
        }

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
            case 's':
                if (num1 >= 0) {
                    result = std::sqrt(num1);
                } else {
                    std::cout << "Error: Square root of a negative number is not allowed." << std::endl;
                    continue; // Repeat the loop
                }
                break;
            case 'i':
                if (num1 != 0) {
                    result = 1 / num1;
                } else {
                    std::cout << "Error: Inverse of zero is not allowed." << std::endl;
                    continue; // Repeat the loop
                }
                break;
            case 'a':
                result = std::abs(num1);
                break;
            case 'l':
                if (num1 > 0 && num2 > 0) {
                    result = std::log(num1) / std::log(num2);
                } else {
                    std::cout << "Error: Logarithm arguments must be positive." << std::endl;
                    continue; // Repeat the loop
                }
                break;
            case 'n':
                if (num1 > 0) {
                    result = std::log(num1);
                } else {
                    std::cout << "Error: Natural logarithm argument must be positive." << std::endl;
                    continue; // Repeat the loop
                }
                break;
            case '%':
                result = std::fmod(num1, num2);
                break;
            default:
                std::cout << "Invalid operation. Please use +, -, *, /, ^, s, i, a, l, n, or %." << std::endl;
                continue; // Repeat the loop
        }

        std::cout << "Result: ";
        if (operation == 's') {
            std::cout << "sqrt(" << num1 << ") = " << result;
        } else if (operation == 'i') {
            std::cout << "1 / " << num1 << " = " << result;
        } else if (operation == 'a') {
            std::cout << "| " << num1 << " | = " << result;
        } else if (operation == 'l') {
            std::cout << "log_" << num2 << "(" << num1 << ") = " << result;
        } else if (operation == 'n') {
            std::cout << "ln(" << num1 << ") = " << result;
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

A lot was added and changed in this iteration, and it created multiple errors. The biggest one is removing the option to set the first number. It is unclear what caused this to happen, but a good guess is that when specifying the program to repeat with the result, the initial setter for the first number was removed. When this was pointed out, a setter for the first number was added and code for prompting the user about using the result from the previous operation was added, but this still had a problem. There was also another problem where log should have been set to base10 as a default; However, this is not a fault of the AI as it was not specified in the directions. When asked to only allow base10 and not require a second number, ChatGPT decided to remove num2 from the entire program and have a new numX input in each case statement that needed it. This created redundant code that could be simplified and when mentioned that the program should contain a second number, it was readded.

The prompts to fix all these areas are as follows: "this does not allow the user to set the initial number", "when printing out log, there should not be a second number as the default is base 10", "you still need a second number in the program, just not in log", "some more operations do not need a second number as well", and "I noticed that when the user does not want to use the previous result, there is no prompt for a new first number".


### Inability to set a new first number

```cpp
if (result != 0.0) {
    char useResult;
    std::cout << "Use the previous result (" << result << ") as the new first number? (y/n): ";
    std::cin >> useResult;
    if (useResult == 'y' || useResult == 'Y') {
	num1 = result;
    }
}
```

While the above code allowed the user to reuse the previous result as the first number, it does not allow the user to set a new first number. It should also be noted that if the result of the previous operation is zero, the following operation will use the previous first number instead of setting a new one.

### Unwanted removal of num2

```cpp
switch (operation) {
	case '+':
		double num2;
		std::cout << "Enter the second number: ";
		std::cin >> num2;
		result = num1 + num2;
		break;
	case '-':
		double num3;
		std::cout << "Enter the second number: ";
		std::cin >> num3;
		result = num1 - num3;
		break;
	case '*':
		double num4;
		std::cout << "Enter the second number: ";
		std::cin >> num4;
		result = num1 * num4;
		break;
```

As you can see in the small sample above, there was a user input in each of the cases that required a second number, which is redundant when you could have one member variable that does the same thing. When mentioned that a second number should still be in the program, the num2 member variable was readded and all redundant inputs were removed.

## Final addition of code

For the last part of adding complexity, an operation for factorial was added. Note that this was after all the previous problems were fixed and the goal is to see if it can add a single semi-complex function into the mix.

The last prompt that was given was: "add an operation for factorial".

```cpp
#include <iostream>
#include <cmath> // For mathematical functions

// Function to calculate the factorial of a non-negative integer
double factorial(int n) {
    if (n < 0) {
        return -1; // Return -1 to indicate an error
    }
    if (n == 0) {
        return 1; // 0! = 1
    }
    double result = 1;
    for (int i = 1; i <= n; ++i) {
        result *= i;
    }
    return result;
}

int main() {
    char repeat;
    double result = 0.0; // Variable to store the result

    do {
        double num1;
        double num2 = 0.0;
        char operation;

        // If there is a previous result, ask the user if they want to use it as the new first number
        if (result != 0.0) {
            char useResult;
            std::cout << "Use the previous result (" << result << ") as the new first number? (y/n): ";
            std::cin >> useResult;
            if (useResult == 'y' || useResult == 'Y') {
                num1 = result;
            } else {
                std::cout << "Enter a new first number: ";
                std::cin >> num1;
            }
        } else {
            std::cout << "Enter the first number: ";
            std::cin >> num1;
        }

        std::cout << "Enter an operation (+, -, *, /, ^ (exponentiation), s (square root), i (inverse), a (absolute value), l (log), n (ln), f (factorial)): ";
        std::cin >> operation;

        // For operations that do not require a second number, set it to 0.0 by default
        if (operation != 's' && operation != 'i' && operation != 'a' && operation != 'l' && operation != 'n' && operation != 'f') {
            std::cout << "Enter the second number: ";
            std::cin >> num2;
        }

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
            case 's':
                if (num1 >= 0) {
                    result = std::sqrt(num1);
                } else {
                    std::cout << "Error: Square root of a negative number is not allowed." << std::endl;
                    continue; // Repeat the loop
                }
                break;
            case 'i':
                if (num1 != 0) {
                    result = 1 / num1;
                } else {
                    std::cout << "Error: Inverse of zero is not allowed." << std::endl;
                    continue; // Repeat the loop
                }
                break;
            case 'a':
                result = std::abs(num1);
                break;
            case 'l':
                if (num1 > 0) {
                    result = std::log10(num1); // Common logarithm (base 10)
                } else {
                    std::cout << "Error: Logarithm argument must be positive." << std::endl;
                    continue; // Repeat the loop
                }
                break;
            case 'n':
                if (num1 > 0) {
                    result = std::log(num1); // Natural logarithm (base e)
                } else {
                    std::cout << "Error: Natural logarithm argument must be positive." << std::endl;
                    continue; // Repeat the loop
                }
                break;
            case 'f':
                if (num1 >= 0 && std::floor(num1) == num1) {
                    result = factorial(static_cast<int>(num1));
                } else {
                    std::cout << "Error: Factorial argument must be a non-negative integer." << std::endl;
                    continue; // Repeat the loop
                }
                break;
            default:
                std::cout << "Invalid operation. Please use +, -, *, /, ^, s, i, a, l, n, or f (factorial)." << std::endl;
                continue; // Repeat the loop
        }

        std::cout << "Result: ";
        if (operation == 's') {
            std::cout << "sqrt(" << num1 << ") = " << result;
        } else if (operation == 'i') {
            std::cout << "1 / " << num1 << " = " << result;
        } else if (operation == 'a') {
            std::cout << "| " << num1 << " | = " << result;
        } else if (operation == 'l') {
            std::cout << "log(" << num1 << ") = " << result;
        } else if (operation == 'n') {
            std::cout << "ln(" << num1 << ") = " << result;
        } else if (operation == 'f') {
            std::cout << num1 << "! = " << result;
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

Looking at this code, it is impressive that ChatGPT created a good setup for a factorial operation. In the case statement for factorial, it checks if the first number is non-negative and an integer before casting the number to an integer for the factorial function. If it is not a positive number, or not an integer, the factorial function does not get run. The real surprise was inside the factorial function where ChatGPT created a factorial algorithm with a space complexity of O(1) by using a loop, which is much more efficient than the recursive algorithm which is O(n). It was a little surprising that after many mistakes along the way, ChatGPT was able to create a new operation and function that was efficient and free from errors.

Whenever errors were pointed out to ChatGPT, it quickly understood the problem and fixed it. Only in one situation did it not fully fix the problem was when removing the second number for the log function and ChatGPT removed the second variable for the entire program. Since every error that was created by ChatGPT in the code had to be individually found, the time that it took to review the code could have been spent on writing it without ChatGPT and may have even been able to save time in the end.

## Conclusion

ChatGPT gets the general idea of creating a calculator correct and needs very specific inputs from the prompter to write good code. It is still important to review all the code that it creates, as it is very prone to even simple errors that beginner programmers can easily avoid. It was surprising that ChatGPT struggled with creating quality code as it is viewed in society as something that could "replace programmers". In actuality, ChatGPT is not close to this as it would need to be able to write efficient and quality code without many errors, which is not the case. It could be good at getting an idea of how to write a function as it can create simple functions, but anything that starts to get more complex would need a programmer to do it.

## References

https://chat.openai.com/share/6ff7db27-ee1c-4067-8abe-ce26c6bf2774

*I did ask a couple of the questions out of order as it appears here but there is an expectation that the code would remain the same regardless*
