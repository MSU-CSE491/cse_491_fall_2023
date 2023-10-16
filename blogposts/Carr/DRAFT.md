# Lambdas in C++

## About Lambdas
Lambdas are local function objects (classes) defined "on the fly" that are useful for short snippets of code that are not necessary elsewhere. Many times lambdas are seen being used in algorithms of the std namespace. Lambdas can be assigned to variables, so that if there are many different uses, there is no need to rewrite the lambda code - the variable can be used. Lambdas have the ability to be for specific types or generic (can be for many different types). The main parts of a lambda are the capture clause, the parameter list, mutable specification, exception specification, and lambda body. In the following sections, examples will be worked through to further understanding of lambdas and their anatomy. 

## Basic Lambda Example
```c++
int main() {
    // define vector
    std::vector<int> v {1, 29, -10, 32, 11, -4, 17, 4, 2, 9};

    // find first element that is less than 0
    auto it = std::find_if(v.begin(), v.end(),
                           [] (int num) {
                                return num < 0;
                           });
    
    // print out element that is less than 0
    std::cout << *it << std::endl;    

    return 0;
}
```
In this example, a simple lambda is used with the find_if std algorithm to find the first element that is less than zero. The lambda in this example has nothing in its capture clause, has no mutable specification, and no exception specification. The parameter list simply takes in an integer, as the vector is a vector of integers. The lambda body is a simple return statement that checks if the integer is less than zero.  

## Lambda as a Variable Example
```c++
int main() {
    // define vector
    std::vector<int> v {1, 29, -10, 32, 11, -4, 17, 4, 2, 9};

    // create lambda and store it in a variable
    auto lessThanZero = [] (int num) {
        return num < 0;
    };

    // find first element that is less than 0
    auto it = std::find_if(v.begin(), v.end(), lessThanZero);

    // count all elements that are less than 0
    auto count = std::count_if(v.begin(), v.end(), lessThanZero);
    
    // print out element that is less than 0
    std::cout << "First element less than 0: " << *it << std::endl;  

    // print out number of elements less than 0
    std::cout << "Number of elements less than 0: " << count << std::endl;  

    return 0;
}
```
Lambdas can be assigned to variables. This is useful when there are multiple uses for the same lambda in a single scope. By assigning the lambda to a variable, the lambda is only written one time, but can be used many times in that scope. This results in much cleaner code. In this example, a lambda is used to find the first element that is less than zero, and also count the number of elements that are less than zero in the vector v. As seen in vector v, the first element less than zero is -10, and the number of elements less than zero is 2 (-10 and -4).

## Using Lambda Capture
```c++
int main() {
    // define vector
    std::vector<int> v {1, 29, -10, 32, 11, -4, 17, 4, 2, 9};

    // define min and max
    int min = 8;
    int max = 15;

    // create lambda and store it in a variable
    auto inRange = [min, max] (int num) {
        return num < max && num > min;
    };

    // find first element that is in range
    auto it = std::find_if(v.begin(), v.end(), inRange);

    // count all elements that are in range
    auto count = std::count_if(v.begin(), v.end(), inRange);
    
    // print out element that is in range
    std::cout << "First element in range (8, 15): " << *it << std::endl;  

    // print out number of elements in range
    std::cout << "Number of elements in range (8, 15): " << count << std::endl;  

    return 0;
}
```
This example is similar to the previous example. However, in this case, the lambda capture is being used. Captures in lambdas (the []) allow the lambda to access variables defined in the same scope as the lambda. The lambda is no longer limited to variables passed in as parameters/variables defined inside of the lambda body. There is a min and max defined in the same scope as the lambda, and these are the two values seen in the capture - [min, max]. Now, instead of having a lessThanZero lambda, it is an inRange lambda, which uses the min and max to find the first element in range and count the number of elements in range for vector v. 

## Using Lambda Capture and Mutable Specification
```c++
int main() {
    // define string
    std::string message = "Hello";

    // define lambda
    auto improveMessage = [&message] (std::string mess) mutable {
        message += mess;
    };

    // call lambda to improve message
    improveMessage(" my name is Grant. Welcome to the C++ Lambdas Blogpost!");

    // look at new message!
    std::cout << message << std::endl;
}
```
Specifying a lambda as mutable is a sometimes useful tool when working with captures. In certain situations, it may be necessary/helpful to be able to modify values captured by the lambda. In the example above, a lambda is used to improve a message. Due to the lambda capturing the string **message** by reference, any change done to it inside the lambda body modifies the variable outside of the lambda as well. After using this lambda, the string **message** will contain the text "Hello my name is Grant. Welcome to the C++ Lambdas Blogpost!". However, if instead the lambda were to look like this:
```c++
    auto improveMessage = [message] (std::string mess) mutable {
        message += mess;
    };
```
The string **message** would **only** be modified inside of the lambda body. Outside of the lambda, since **message** is not captured by reference, it would still only contain "Hello". It is important to understand these rules when specifying a lambda as mutable and using the lambda capture:
* Variables captured by **reference** and **mutable** lambda - if the variables captured by the lambda are modified in the lambda body, they will be modified outside the lambda as well. 
* Variables captured **normally** and **mutable** lambda - if the variables captured by the lambda are modified in the lambda body, they will **only be modified inside the lambda.** The variables will still hold their original value after/outside the lamdba.

## Generic Lambdas
```c++
int main() {
    // generic lambda
    auto combineOnce = [] (auto var) {
        return var + var;
    };

    std::cout << "Combining integers: " << combineOnce(3) << std::endl;
    std::cout << "Combining floats: " << combineOnce(2.2) << std::endl;
    std::cout << "Combining string: " << combineOnce(std::string{"Grant"}) << std::endl;
}
```
Lambdas have the ability to be generic. Similar to templated functions, lambdas can act almost as if it were a templated function. By making the parameters type **auto**, lambdas are able to handle many different types. In this example, strings, floats, and integers are all able to work with the lambda function **combineOnce**. One important thing to understand is this lambda will only work with types that can be added together. The types a generic lambda are sufficient for depend on the operations occurring inside the lambda body. 

## Lambdas as Constexpr
```c++
int main() {
    // generic lambda as constexpr
    constexpr auto sum = [] (const auto &var_one, const auto &var_two) {
        return var_one + var_two;
    };

    std::cout << "Summing integers: " << sum(3, 7) << std::endl;
    std::cout << "Summing floats: " << sum(2.2, 3.3) << std::endl;
    std::cout << "Summing string: " << sum(std::string{"Grant"}, std::string{" Lambda"}) << std::endl;
}
```
Another interesting feature of lambdas is they can be declared as constexpr. This declares that it is possible to evaluate the value of the lambda at compile time. The benefits of computing at compile time rather than runtime is that it helps the program run faster and use less memory. Lambdas are also implicity constexpr if its result satisfies the requirements of a constexpr function. An example that would cause an error would be declaring a lambda as **constexpr** but then making it **mutable**. If the lambda has any captures, those must also be const. 

## Lambda as Function Parameter
```c++
// templated function that uses a generic lambda and generic types
template <typename F, typename T>
void UseLambda(F lambda, T var1, T var2) {
    std::cout << lambda(var1, var2) << std::endl;
}

int main() {
    // generic lambda as constexpr
    auto sum = [] (const auto &var_one, const auto &var_two) {
        return var_one + var_two;
    };

    // send lambda to function
    UseLambda(sum, 3, 6);
    UseLambda(sum, 3.7, 2.1);
    UseLambda(sum, std::string{"Hello"}, std::string{" World"});
}
```
Another unique property of lambdas is their ability to be sent to other functions as a parameter. As a lambda is really just an object, it can be sent and used in functions where it is not defined. As the lambda type is somewhat unknown (it is declared as **auto**), the function that wants to use the lambda must be a templated function. In this example, the lambda **sum** is a generic lambda - it can work for many different types. The parameters required for the lambda are also sent to the function **UseLambda**, and due to the lambda being generic, these are templated types as well. 

## Wrap Up
Lambdas have many different uses:
* Simple, short, local, inline functions that do not require creating a new function.
* Can be assigned to variables if used in many different locations. This makes code better understandable and improves readability.
* The **capture** of a lambda allows it to use and edit variables outside of the lambda body.
    * Adding the **mutable** specification allows lambdas to change variables captured. 
* Lambdas can be generic. Similar to templated functions, lambdas declared with **auto** type parameters are generic and can work for many different types depending on the operations occurring in the lambda body. 
* Lambdas can be declared and implicitly **constexpr**. This allows the lambda to be evaluated at compile time, possibly speeding up code. 
* Functions can have a parameter that is a lambda. Due to lambdas being an object, they can be sent to other functions to be used in. The function using a lambda as a parameter must be a templated function to allow use. 

