# Coroutines Tutorial
Let's dive into the advanced C++ topic of coroutines, discussing what they are, how they're used, and their importance!

## What are coroutines?
C++ coroutines, introduced in C++20, offer a streamlined approach to asynchronous and concurrent programming. They enable you to craft ***asynchronous code*** that closely resembles ***synchronous programming***, enhancing code clarity and maintainability. 

Coroutines can temporarily pause their execution at designated points, later resuming from where they left off. This pause-and-resume behavior allows other tasks to be executed in the interim. Furthermore, they operate without relying on a traditional stack, suspending execution by returning to the caller and storing the necessary data separately from the stack. In essence, this stackless mechanism stores the coroutine's data, or coroutine frame, on the heap.

Below is a great example of the coroutine functionality: 

<p align="center">
  <img src="https://i.stack.imgur.com/fNBq8.png" alt="Screenshot of basic coroutine functionality"/>
</p>


## Different types of coroutines
Generally, coroutines can be categorized into two primary types:
+ Generator coroutine: designed for producing sequences
+ Task coroutine: employed for asynchronous programming


### Generator coroutines
Generator coroutines exhibit a unique behavior. They generate sequences of values in a lazy, on-demand manner, yielding each value individually when requested by the consumer. This is achieved through the use of the <ins>co_yield</ins> keyword, which enables you to generate values without the need to compute the entire sequence in advance. Generator coroutines are particularly valuable when implementing iterators or implementing a mechanism for efficient lazy sequence processing. Below is an example of a simple generator coroutine.

```C++
#include <iostream>
#include <coroutine>

generator<int> GenerateNumbers(int from, int to) {
  for (int i = from; i <= to; ++i) {
    co_yield i;
  }
}

int main() {
  for (int number: GenerateNumbers(1, 5)) {
    std::cout << number << " ";
  }
}
```
In this example, we define the generator coroutine called generateNumbers that yields a sequence of numbers using co_yield with “from” and “to”.

Output:
```
1 2 3 4 5
```

### Task coroutines
On the other hand, task coroutines serve the purpose of enabling you to await asynchronous operations without causing the calling thread to become blocked. These coroutines make extensive use of the co_await keyword, which effectively pauses their execution until the awaited operation has concluded. Various operations that can be awaited encompass I/O tasks (which we'll delve into shortly), network requests, and timers. To illustrate the concept, here's a straightforward task coroutine that emulates an asynchronous operation.

```C++
#include <iostream>
#include <coroutine>
#include <chrono>

std::task<void> AsyncTask() {
  // Simulate an asynchronous task
  std::cout << "Async task started." << std::endl;

  // Simulate an asynchronous operation
  co_await std::suspend_for(std::chrono::seconds(2));

  std::cout << "Async task completed." << std::endl;
}

int main() {
  // Start the asynchronous task using 'resume'
  AsyncTask().resume();

  // Continue with other work while the async task is in progress
  std::this_thread::sleep_for(std::chrono::seconds(3));

  std::cout << "Main function continues to run asynchronously." << std::endl;

  return 0;
}

```

We define a task coroutine called asyncTask that simulates an asynchronous operation using std::suspend_for. Using the co_await keyword to suspend execution, we allow other tasks to run concurrently during the delay. In the main function, we start the coroutine by calling resume(), and while the async task is in progress, the main function continues to execute other code concurrently.

Output:

```
Async task started.
Main function continues to run asynchronously.
Async task completed.
```

## How do we implement coroutines?

+ Must be using C++20
+ Include the coroutine header --> ```#include <coroutine>```
+ Define the coroutine function

### Defining a coroutine function
The cppreference definition states, “a function is a coroutine if its definition contains any of the following:”
+ ```co_await``` – for suspending execution until resumed
+ ```co_yield``` – for suspending execution and returning a value
+ ```co_return``` – for completing execution and returning a value

### Return types
Coroutines can have different return types – similar to a regular function – that depend on the specific behavior you want to achieve.

```void``` – when the coroutine doesn’t return any meaningful result
  + suitable for coroutines that primarily have side effects or perform asynchronous operations without producing a value
 
```std::suspend_never``` - when you want the coroutine to run to completion without ever suspending
  + appropriate for lightweight, non-blocking tasks that don’t need to wait for other operations
  
```std::suspend_always``` – when you want the coroutine to suspend immediately upon entry and never resume
  + useful for cases when you don’t intend to perform any work in the coroutine
 
```std::coroutine_handle<>``` - a low-level coroutine used for customizing coroutine handling

```std::task<T>``` or ```std::task<void>``` - should be used for high level asynchronous programming
  + suitable for representing asynchronous operations as tasks and can be used with libraries that provide abstractions
 
```custom promise types``` - for more complex coroutines that need to return values, manage state, or implement advanced coroutine behavior


## Custom coroutines
As mentioned earlier, custom coroutines come in handy if we want to return specific values, manage the state, or even implement advanced coroutine behavior. Let's jump into everything you need in order to accomplish this.

### Define the promise_type structure or class 
The promise_type is a structure associated with a coroutine that is responsible for managing the coroutine’s state and returning the final result. It allows developers to define how a coroutine behaves during its lifetime, such as managing resources, handling exceptions, and controlling the flow of execution.
This structure encapsulates the logic related to the coroutine's lifetime and interactions, providing a means for developers to tailor coroutine behavior to specific application requirements.

Now let's get into the essential and commonly used members

🔴 REQUIRED
🟢 TYPICALLY REQUIRED

#### Essential members:
🔴```get_returned_object()``` - returns an instance of the object (this is where the coroutine’s result is stored)
    
🔴```unhandled_exception()``` – for proper exception handling within the coroutine

🟢```return_void()``` – allows the promise to finalize the coroutine’s results **(required if coroutine returns void)**
 
🟢```initial_susend()``` – specifies whether the coroutine should suspend immediately upon entering by deciding whether to perform some initial setup or suspend right away 
    + use when you want to control the suspension behavior of the coroutine when it starts
   
🟢```final_suspend()``` – specifies whether the coroutine should suspend after the last value (generator), or when the coroutine is complete
    + use when you want to control the suspension behavior of the coroutine after producing all values or completing its task
   
#### Commonly used members:
```yield_value()``` - allows the coroutine to yield a value using <ins>co_yield</ins>
    + most common in generator coroutines
      
```await_suspend()``` – allows custom logic to be applied when the coroutine is suspended
 
```await_resume()``` – used when the coroutine resumes after a "suspend", usually to retrieve the result of the awaited tasks
 
```get_return_object_on_allocation_failure()``` – used to handle resource allocation failures when creating the coroutine promise; can provide an alternative object in case of allocation failure
   
```rethrow_if_nested(task<...>)``` – used for proper exception handling in nested tasks; ensures that exceptions thrown in nested tasks are rethrown correctly
      
<br><br>
Now that we have the basics covered on customizable coroutines, let's dive into the first example that designs a chat coroutine!
<br>

## Custom chat example

```C++
#include <iostream>
#include <coroutine>
#include <string>

struct Chat {

    struct promise_type {
        std::string msgOut{}, msgIn{}; // this stores values going into or coming out of the coroutine

        void unhandled_exception() noexcept {}; // what to do when there's an exception
        Chat get_return_object() { return Chat(this); } // coroutine creation
        std::suspend_always initial_suspend() noexcept { return{}; } // startup
        std::suspend_always yield_value(std::string msg) noexcept // value from co_yield
        {
            msgOut = std::move(msg);
            return {};
        }

        auto await_transform(std::string) noexcept // value from co_await
        {
            struct awaiter { //> these can be customized instead of using suspend_always or suspend_never
                promise_type& pt;
                constexpr bool await_ready() const noexcept { return true; }
                std::string await_resume() const noexcept { return std::move(pt.msgIn); }
                void await_suspend(std::coroutine_handle<>) const noexcept {}

            };

            return awaiter{*this};
        }

        void return_value(std::string msg) noexcept { msgOut = std::move(msg); } // value from co_return
        std::suspend_always final_suspend() noexcept { return {}; }  // ending

    };
```

We create a coroutine called Chat with the promise_type struct nested inside. At the bottom, the chat coroutine has a return_value function that is callable from the co_return keyword. We also set final_suspend to suspend_always, meaning when the coroutine is finished, it will always suspend, or in other words, terminate.


Taking a look into the promise_type struct, you will notice it’s <ins>nested</ins> inside the chat coroutine. We include member variables to store values that will come in and go out of the coroutine. 
As mentioned before, we include the <ins>essential member functions</ins>:
+ ```C++
  void unhandled_exception() noexcept {};
  ```
    handles any exceptions
  
+ ```C++
  Chat get_return_object() { return Chat(this); }
  ```
  returns the coroutine object when first created

+ ```C++
  std::suspend_always initial_suspend() noexcept { return{}; }
  ```
    the coroutine is initially suspended before it starts executing, implying the coroutine will need to be explicitly resumed using the ```resume()``` member function (here)
  
+ ```C++
    std::suspend_always final_suspend() noexcept { return {}; }
  ```
    terminates the coroutine when finished
  

You will also noticed the <ins>commonly used member functions</ins> mentioned before:
<br>

+ ```C++
  std::suspend_always yield_value(std::string msg) noexcept // value from co_yield
    {
        msgOut = std::move(msg);
        return {};
    }
  ```
    which stores the value from the <ins>co_yield</ins> keyword into the msgOut member variable. Additionally, the await_transform member function is part of the coroutine’s promise type, and can be called using the <ins>co_await</ins> keyword. Inside the await_transform function contains a local awaiter structure which is responsible for controlling the behavior of the coroutine when co_await is used. You will notice the awaiter object has:

+ ```C++
  promise_type& pt;
  ```
    the awaiter struct constructor takes a reference to the promise type allowing the awaiter to access the state and control the behavior of the coroutine

+ ```C++
  constexpr bool await_ready() const noexcept { return true; }
  ```
    called when the co_await expression is evaluated; will return a boolean indicating whether the awaited value is immediately available

+ ```C++
  std::string await_resume() const noexcept { return std::move(pt.msgIn); }
  ```
    called when the coroutine is ready to resume after suspension. In this case, it retrieves the awaited value from the coroutines promise type, specifically moving pt.msgIn and returning it as a string

+ ```C++
  void await_suspend(std::coroutine_handle<>) const noexcept {}
  ```
    called when the coroutine is about to be suspended after the co_await expression is evaluated. In this case, it doesn’t perform any specific suspension action

<br>
Moving on, we will look more closely at the chat structure

```C++
#include <iostream>
#include <coroutine>
#include <string>

struct Chat {

    struct promise_type {...};

    using Handle = std::coroutine_handle<promise_type>;  // shortcut
    Handle mCoroHdl{};

    explicit Chat(promise_type *p) : mCoroHdl{Handle::from_promise(*p)} {} // get the handle from the promise
    Chat(Chat && rhs) : mCoroHdl{std::exchange(rhs.mCoroHdl, nullptr)} {} // move only

    /// Destructor
    ~Chat()
    {
        if (mCoroHdl) { mCoroHdl.destroy(); }
    }

    std::string listen() // activate the coroutine and listen
    {
        if (not mCoroHdl.done()) { mCoroHdl.resume(); }
        return std::move(mCoroHdl.promise().msgOut);
    }

    void answer(std::string msg) const // send data to the coroutine and activate it
    {
        mCoroHdl.promise().msgIn = msg;
        if (not mCoroHdl.done()) { mCoroHdl.resume(); }
    }

};
```

We use the
```C++
using Handle = std::coroutine_handle<promise_type>;
Handle mCoroHdl{};
```
which allows us to customize our coroutine handling to create an alias named Handle and declare mCoroHdl as a member variable of type Handle which will be used to control the execution of the coroutine


We create a Chat instance and initialize mCoroHdl with the coroutine handle created from the provided promise:
```C++
  explicit Chat(promise_type *p) : mCoroHdl{Handle::from_promise(*p)} {}
```
Additionally, we create the move constructor for Chat that takes an rvalue reference to another Chat instance and moves the coroutine handle from rhs to the current object:
```C++
  Chat(Chat && rhs) : mCoroHdl{std::exchange(rhs.mCoroHdl, nullptr)}
```

Although it’s not necessary, it can be beneficial if you want to optimize memory and performance, are worried about resource management, or just need to transfer ownership. For instance, if your object is managing a large buffer, it would be highly beneficial to transfer ownership rather than copying it

Continuing, we create a listen function which is used to activate the coroutine and listen for a response. It first checks if the coroutine is done, if not, it calls resume() to advance the coroutine’s execution. In this example, it returns the restored response in the promise’s msgOut member:
```C++
  std::string listen()
    {
        if (not mCoroHdl.done()) { mCoroHdl.resume(); }
        return std::move(mCoroHdl.promise().msgOut);
    }
```

Lastly, we create an answer function used to send data to the coroutine and activate it by storing the provided message in the promise’s msgIn member and again check if the coroutine is done. If not, it resumes:
```C++
  void answer(std::string msg) const
    {
        mCoroHdl.promise().msgIn = msg;
        if (not mCoroHdl.done()) { mCoroHdl.resume(); }
    }
```
<br>
Let's get into the last part of this chat coroutine

```C++
Chat Fun()
{
    co_yield "Hi, what's your name?\n"; // suspends coroutine w/ output message
    std::string name = co_await std::string{}; // suspends coroutine w/ input message
    co_return "Nice to meet you, " + name + "!\n"; // ends coroutine and returns final message

}

int main()
{
    std::string input;
    Chat chat = Fun();
    std::cout << chat.listen();

    std::getline(std::cin, input);
    chat.answer(input);

    std::cout << chat.listen();
    return 0;

}
```

Finally, we define the Fun() coroutine function. We create a Chat object named chat and begin the coroutine’s execution and the initial message is displayed with ```chat.listen()```. We use ```chat.answer()``` to send the user’s input as a response to the coroutine which will continue executing after the co_await line on line 65. And at the end, we call ```chat.listen()``` activating the coroutine to return the final message.

This is a great example of how a coroutine can be used to simulate the conversation, with the coroutine yielding messages, awaiting user input, and finally returning a response. 

<br>

## Custom vector example
Here is another example of a customizable generator coroutine that interleaves two vectors:

```C++
#include <iostream>
#include <coroutine>
#include <vector>

struct Generator {

    struct promise_type {
        int val{};

        Generator get_return_object() { return Generator(this); }
        std::suspend_never initial_suspend() noexcept { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        std::suspend_always yield_value(int v)
        {
            val = v;
            return {};
        }
        void unhandled_exception() {}
    };

    using Handle = std::coroutine_handle<promise_type>;
    Handle mCorohdl{};

    explicit Generator(promise_type* p) : mCorohdl{Handle::from_promise(*p)} {}

    Generator (Generator&& rhs) : mCorohdl{std::exchange(rhs.mCorohdl, nullptr)} {}
    ~Generator()
    {
        if (mCorohdl) { mCorohdl.destroy(); }
    }

    int value() const { return mCorohdl.promise().val; }
    bool finished() const { return mCorohdl.done(); }
    void resume()
    {
        if (not finished()) { mCorohdl.resume(); }
    }
};

Generator interleaved(std::vector<int> a, std::vector<int> b)
{
    auto lamb = [](std::vector<int>& v)->Generator {
        for (const auto &e : v) {
            co_yield e;
        }
    };

    auto x = lamb(a);
    auto y = lamb(b);

    while (not x.finished() or not y.finished()) {
        if (not x.finished()) {
            co_yield x.value();
            x.resume();
        }

        if (not y.finished()) {
            co_yield y.value();
            y.resume();
        }
    }
}

int main()
{
    std::vector a{1, 3, 5, 7, 9};
    std::vector b{2, 4, 6, 8, 10};

    Generator g(interleaved(std::move(a), std::move(b)));

    while (not g.finished())
    {
        std::cout << g.value() << "\n";
        g.resume();
    }

    std::cout << std::endl;
    return 0;
}
```

#### Generator struct
+ Custom coroutine type designed to generate and yield values
+ Defines the promise_type struct ultimately customizing the coroutine behavior
+ The generator type uses a coroutine handle (std::corourtine_handle) to manage the coroutine’s execution
+ Provides a method to check if the coroutine is finished, obtain the current yielded value, and manually resume the coroutine
+ Struct manages the destruction of the coroutine handle in its destructor

#### Interleaved function
+ Takes two vectors as input (a & b)
+ Inside the function, it defines the lambda function that takes a reference to the vector and creates a Generator for it
+ The lambda iterates through the input vector and yields each element in the vector using co_yield
+ Interleaves the values from the two Generator instances, x and y, in the while loop
+ Yields values from x and y and if either x or y are not finished, it resumes the respective generator

#### Main function
+ The two vectors are defined (a & b)
+ Interleaved function is called and its results are stored in a generator called g
+ The values are printed to the console, and the generator is manually resumed


## Why use coroutines?
Coroutines provide a more efficient and manageable way to achieve concurrency, making them valuable for tasks involving parallelism, scalability, and asynchronous operations. They contribute to cleaner and more readable code while avoiding some of the challenges associated with traditional multithreading approaches.

They are particularly useful for handling requests or interacting with databases ultimately improving the responsiveness of applications. When a coroutine encounters an I/O operation, it can yield control back to the event loop, allowing other tasks to run which was shown in both the custom examples. 

Coroutines are often integrated with event loops, which are a fundamental component of event-driven programming. The event loop manages the scheduling and execution of coroutines, ensuring that events are processed efficiently and that the application remains responsive

Additionally, if you are worried about resources such as memory, coroutines are extremely useful for resource management. Coroutines are more resource-efficient than creating a thread or process for every asynchronous task. They can be executed on a limited number of threads, which saves memory and resources. 

<br><br>
🎉🎉🎉Congrats! You've finished the tutorial!🎉🎉🎉

I hope this tutorial was helpful and you have a better understanding of C++ coroutines!


## References
https://en.cppreference.com/w/cpp/language/coroutines

https://stackoverflow.com/questions/71153205/c-coroutine-when-how-to-use

https://www.scs.stanford.edu/~dm/blog/c++-coroutines.html

https://www.geeksforgeeks.org/coroutines-in-c-cpp/

ChatGPT
