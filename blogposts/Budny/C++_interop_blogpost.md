# Problem setup. 
Suppose you've created a program in C++. It works well, you've tested it, it meets all your requirements, maybe there's bugs but you haven't found any recently. And you move on to a different project, and that project happens to be in a different language, but then you realize you've already solved the problems you're facing in this new project in your C++ program. Wouldn't it be nice if you could re-use that code, somehow?

Luckily, it IS often possible to re-use that code, especially if it's something like a library that's naturally suited to re-use. The language barrier is only a minor hurdle you need to clear – many modern languages have support for calling functions in other languages via something known as a Foreign Function Interface (FFI). 

Some languages support C++ interoperability directly, but many languages' FFIs are designed to adapt C functions instead. This still allows you to use C++ code, but it requires you to wrap your C++ code with a C interface. Thus, the first step in allowing your C++ code to be used elsewhere is sometimes to take a brief break from writing C++, and switch to writing an interface in the style of C.

# Creating our C++ code
Suppose you have designed an incredible C++ library, and you want to call it from a language that only supports a C FFI, like Python.

```cpp
// File cool.hpp
#ifndef MY_COOL_CLASS
#define MY_COOL_CLASS

class myCoolClass {
	myCoolClass() { /* construct whatever */ }
	
	void doSomething();
	int doSomethingElse(int);
};

#endif // MY_COOL_CLASS
```

Clearly this class must be very important and useful, so let's see the implementation file:

```cpp
// File cool.cpp
#include "cool.hpp"

#include <iostream>

void MyCoolClass::doSomething(){
	std::cout << "Hello from C++!" << std::endl;
}

int MyCoolClass::doSomethingElse(int a){
	std::cout << a << std::endl;
	return a*a;
}
```

Incredible. This will surely be useful later.

If we were writing a program to use this in C++, nothing here is very difficult.

```cpp
// File main.cpp
#include "cool.hpp"

int main(){
	MyCoolClass c{};
	
	c.doSomething();
	int r = c.doSomethingElse(2);
	c.doSomethingElse(r);
}
```

And we can compile with something like the following:

```
g++ main.cpp cool.cpp -o coolfile
```

This is all nice and easy, but we don't want to write our primary program in C++. Suppose we wanted to call this code from Python 3 instead. It's such a useful library after all, who wouldn't want to use it from other languages?

# Creating a C API
The next step is to revise our C++ source to expose a C API. One way to do that could be the following, using `extern "C"`. This tells the linker to expose these functions as C functions, instead of C++ functions. C++ renames functions internally to support things like function overloads; `extern "C"` disables this. Since we will be treating this like a C API, it is necessary to expose the functions as C functions.

In our case, we could add the following to our header file, just after the class definition:

```cpp
// File cool.hpp
class MyCoolClass { /* ... */ };

extern "C" {
	MyCoolClass* MyCoolClass_create();
	void MyCoolClass_doSomething(MyCoolClass*);
	int MyCoolClass_doSomethingElse(MyCoolClass*, int);
	void MyCoolClass_destroy(MyCoolClass*);
}
```

Of course, we need to modify our implementation as well to support this new API. This will usually just be a bunch of function wrappers, with a hint of memory management sprinkled in for those constructors and destructors.

```cpp
// File cool.cpp

MyCoolClass* MyCoolClass_create(){
	return new MyCoolClass();
}

void MyCoolClass_doSomething(MyCoolClass* cc){
	cc->doSomething();
}

int MyCoolClass_doSomethingElse(MyCoolClass* cc, int argument){
	return cc->doSomethingElse(argument);
}

void MyCoolClass_destroy(MyCoolClass* cc){
	delete cc;
}
```

Notice that, because we are dealing with a C API for our C++ program, we are using raw pointers and in this case are manually managing memory. Be careful - it's easy to miss something here and cause a memory leak or a crash!

# Shared objects / DLLs
Python expects a dynamically linked library (DLL) or shared object, so we need to compile as a DLL or shared object. With g++, this looks like:

```
g++ -fPIC cool.cpp -shared -o cool.so
```

The two important flags here are `-fPIC`, which specifies that we are compiling position-independent code, and `-shared`, which specifies that we are making a shared object. Our output file here will be `cool.so`.

It's not all that different from compiling a regular executable.

# Using Python's ctypes module

On the Python side, we might now attempt to construct a program with similar behavior our C++ main program. Notice that we need to load the shared object/DLL from before here:

```python3
// File main.py
import ctypes
from ctypes import cdll

cool = cdll.LoadLibrary("./cool.so")

cc = cool.MyCoolClass_create()

cool.MyCoolClass_doSomething(cc)

res = cool.MyCoolClass_doSomethingElse(cc, 2)
cool.MyCoolClass_doSomethingElse(cc, res)

cool.MyCoolClass_destroy(cc)
```

We can then run this with something like `python3 main.py`, load our library, and... oh, it doesn't work?

```
Hello from C++!
2
4
Segmentation fault (core dumped)
```

The ctypes module is useful, but it isn't magic. Most importantly, it assumes that your arguments and return types are all integers, but we want to work with an object here, which means a pointer. To resolve this, we need to specify the function arguments and returns.

```python3
// File main.py
// Imports here

# Setup here: arugment and return types
# Otherwise it defaults to int for everything, which can and will break pointers
cool.MyCoolClass_doSomething.argtypes = []
cool.MyCoolClass_create.restype = ctypes.c_void_p

cool.MyCoolClass_doSomething.argtypes = [ctypes.c_void_p]
cool.MyCoolClass_doSomething.restype = None

cool.MyCoolClass_doSomethingElse.argtypes = [ctypes.c_void_p, ctypes.c_int]
cool.MyCoolClass_doSomethingElse.restype = ctypes.c_int

cool.MyCoolClass_destroy.argtypes = [ctypes.c_void_p]
cool.MyCoolClass_destroy.restype = None

// Rest of code here
```

Now, our output is what we would expect:

```
Hello from C++!
2
4
```

# Cleanup

Of course, this doesn't look very nice on the Python side now. It suffers from
the same problem that our C++ code had, which is that now you have to interact
with a C-style API. Since this example deals with a library, it might be useful
to now create a wrapper on the Python side as well. It is quite simple to now
wrap our ctypes C wrapper in a Python class...

```python3
// File cool.py
import ctypes
from ctypes import cdll

cool = cdll.LoadLibrary("./cool.so")

# Setup here: arugment and return types
# (defaults to int for everything, which can break pointers)
cool.MyCoolClass_doSomething.argtypes = []
cool.MyCoolClass_create.restype = ctypes.c_void_p

cool.MyCoolClass_doSomething.argtypes = [ctypes.c_void_p]
cool.MyCoolClass_doSomething.restype = None

cool.MyCoolClass_doSomethingElse.argtypes = [ctypes.c_void_p, ctypes.c_int]
cool.MyCoolClass_doSomethingElse.restype = ctypes.c_int

cool.MyCoolClass_destroy.argtypes = [ctypes.c_void_p]
cool.MyCoolClass_destroy.restype = None

# Wrapper class for C wrapper API for our C++ class
class MyCoolClass:
	def __init__(self):
		self._cc = cool.MyCoolClass_create()
	
	def doSomething(self):
		cool.MyCoolClass_doSomething(self._cc)
		
	def doSomethingElse(self, value):
		return cool.MyCoolClass_doSomethingElse(self._cc, value)
		
	def __del__(self):
		cool.MyCoolClass_destroy(self._cc)
```

This finally leaves our main file, now much more closely resembling our C++ main function as we have restored the object-oriented form of our library:

```python3
// File main.py                     # // File main.cpp
from cool import MyCoolClass        # #include "cool.hpp"
                                    # 
                                    # int main(){
c = MyCoolClass()                   #     MyCoolClass c{};
                                    #     
c.doSomething()                     #     c.doSomething();
res = c.doSomethingElse(2)          #     int res = c.doSomethingElse(2);
c.doSomethingElse(res)              #     c.doSomethingElse(4);
                                    # }
```

# Conclusion

This may have seemed like a lot of work, most of which was repeating information we already had.
Naturally, there are several libraries that automate some or most of this process. Determining which library you may wish to use is left as an exercise to the reader.

Even so, there is a clear advantage to being able to re-use code you've already created. And, as the scope of a project grows, the proportion of overhead to functionality decreases as well.

Thanks for reading, and don't forget to watch, star, and fork the repo!

# Useful resources

* https://isocpp.org/wiki/faq/mixing-c-and-cpp - information on `extern "C"`, as well as mixing C and C++ in general
* https://docs.python.org/3/library/ctypes.html - information about the ctypes module, including all the supported types
* https://en.wikipedia.org/wiki/Foreign_function_interface - An overview of foreign function interfaces, including descriptions for many languages
* https://stackoverflow.com/questions/145270/calling-c-c-from-python#145649 - A similar guide used as a refernce when starting this one. Several other answers list potential libraries to use as well.
