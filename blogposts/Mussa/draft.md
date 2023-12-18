## Memory management library in C++
**Author:** *Yones Mussa*

##Introduction:
C++ is a powerful language, but it can also be complex, especially when it comes to memory management. If you are not careful, you can easily end up with memory leaks, which can lead to performance problems and crashes. To help with memory management, C++ provides several smart pointer classes. 

Understanding smart pointers, specifically `unique_ptr` and `shared_ptr`, is vital for efficient and safe memory management in modern C++ development. This means that you do not have to worry about manually deallocating memory when you are finished with it. These classes have different ownership semantics, and they should be used in different situations.

### Ownership semantics
**unique_ptr:** A `unique_ptr` can only own one object at a time. When a `unique_ptr` goes out of scope, the object it owns is automatically deleted.

**shared_ptr:** A `shared_ptr` can own one or more objects at a time. Multiple `shared_ptr` objects can point to the same object. When the last `shared_ptr` to an object goes out of scope, the object is automatically deleted.

Knowing when to use `unique_ptr` and `shared_ptr` is essential in C++ programming. Use `unique_ptr` when you require sole ownership of an object, particularly for managing dynamically allocated local variables.On the other hand, opt for `shared_ptr` when you need to share ownership of an object across multiple pointers. 

By understanding these distinctions, you can make informed decisions about which smart pointer to employ, ensuring efficient memory management and minimizing potential pitfalls in your C++ projects.

**When to use `unique_ptr`:**
You should use `unique_ptr` when you want to have exclusive ownership of an object. For example, you might use `unique_ptr` to manage the lifetime of a local variable that is allocated dynamically.

Here is an example of how to use `unique_ptr`:

```cpp
#include <memory>

int main() {
  // Create a unique_ptr to a new Rectangle object.
  std::unique_ptr<Rectangle> rectangle(new Rectangle(10, 5));

  // Use the rectangle object.
  std::cout << rectangle->area() << std::endl; // Prints 50

  // When the unique_ptr goes out of scope, the rectangle object will be automatically deleted.
}
```
**When to use shared_ptr:**

You should use `shared_ptr` when you need to share ownership of an object with multiple pointers. For example, you might use `shared_ptr` to manage the lifetime of an object that is passed to a function or returned from a function.

Here is an example of how to use `shared_ptr`:


```cpp
#include <memory>

std::shared_ptr<Rectangle> createRectangle() {
  // Create a shared_ptr to a new Rectangle object.
  return std::make_shared<Rectangle>(10, 5);
}

int main() {
  // Get a shared_ptr to a rectangle object.
  std::shared_ptr<Rectangle> rectangle = createRectangle();

  // Use the rectangle object.
  std::cout << rectangle->area() << std::endl; // Prints 50

  // Pass the shared_ptr to another function.
  doSomethingWithRectangle(rectangle);

  // The rectangle object will be automatically deleted when the last shared_ptr to it goes out of scope.
}
```
###Pitfalls

Using `unique_ptr` and `shared_ptr` in C++ offers significant advantages in managing memory and resources. However, there are pitfalls that developers should be aware of. One common issue is the occurrence of dangling pointers, which happens when a smart pointer points to an object that has been deleted or has gone out of scope
- `Dangling pointers:` A dangling pointer is a pointer that points to an object that has already been deleted

- `Circular references:` A circular reference is a situation where two or more objects point to each other

It's crucial to be mindful of these pitfalls and structure your code in a way that avoids these issues, ensuring robust memory management in your C++ applications.

###Conclusion
Mastering smart pointers like `unique_ptr` and `shared_ptr` is crucial for effective and secure memory management in modern C++ development. `unique_ptr` is employed when exclusive ownership of an object is needed, especially for dynamically allocated local variables, whereas  `shared_ptr` is chosen for scenarios requiring shared ownership across multiple pointers. 

By understanding these these differences, developers are better equipped to choose the right smart pointer for their project, ensuring efficient memory management and avoiding potential pitfalls. Always check the smart pointer's validity before using it to avoid common pitfalls l,ike dangling pointers which can lead to crashes. Additionally, use caution when creating connections between objects using shared pointers. It's advised to use other pointers in these situations to avoid circular references, which frequently result in memory leaks.

Smart pointers offer automatic memory management, prevent memory leaks, and enhance code readability and maintainability, making them a best practice in C++ development for crafting efficient and dependable code.