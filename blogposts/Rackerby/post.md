# Fantastic pointers and how to `std::launder` them

C++ has a wide variety of memory-management options, offering many different levels of abstraction. You do so manually using `new` and `delete`, forgoing the need to keep track of how much memory to allocate. You can utilize smart pointers to take advantage of RAII principles to automatically release memory. Even until C++ 23 there was support for [garbage collection](https://en.cppreference.com/w/cpp/memory#Garbage_collector_support_.28until_C.2B.2B23.29).

These high-level abstractions have made memory-management and its associated bugs easier to work with. Modern C++ compilers have been fine-tuned to generate efficient low-level code from these abstractions, and optimizers aid in this step by making assumptions about the operations programmers are allowed to write. However, as a general-purpose systems programming language, C++ must give the programmer access to all levels of abstraction. This includes low levels that allow the user to write programs that violate assumptions the compiler makes. Here, we consider how `std::launder` acts as a back door when the compiler doesn't know how to handle uses of placement `new`.

First, a brief overview of placement `new` and transparent replaceability. The familiar call to `operator new` ([full documentation](https://en.cppreference.com/w/cpp/language/new)) is of the form `new (type) (initializer)`. For example:
```cpp
struct Foo {
    int bar;
    int baz;
}

// Heap-allocate the memory and initialize the object
Foo* a = new Foo{1, 2};
```
 This syntax both allocates memory and initializes it with the supplied arguments. However, if one wishes to decouple the memory allocation from its initialization, a different syntax called placement `new` exists for that purpose. Cppreference provides an example of such:

```cpp
struct C {
    int i;
    void f();
    const C& operator=(const C&);
};
 
const C& C::operator=(const C& other)
{
    if (this != &other) {
        this->~C();          // lifetime of *this ends
        new (this) C(other); // new object of type C created
        f();                 // well-defined
    }
    return *this;
}
 
C c1;
C c2;
c1 = c2; // well-defined
c1.f();  // well-defined; c1 refers to a new object of type C
```

In this example, we've avoided allocating new memory, instead reusing the same memory that was allocated for `c1`. The above operations were well-defined because object `c1` was **transparently replaceable** by `c2`.

According to Cppreference, "If a new object is created at the address that was occupied by another object, then all pointers, references, and the name of the original object will automatically refer to the new object and, once the lifetime of the new object begins, can be used to manipulate the new object, but only if the original object is transparently replaceable by the new object. Object x is *transparently replaceable* by object y if:

- the storage for y exactly overlays the storage location which x occupied
- y is of the same type as x (ignoring the top-level cv-qualifiers)
- x is not a complete const object
    neither x nor y is a base class subobject, or a member subobject declared with [[no_unique_address]](since C++20)
- either 

    - x and y are both complete objects, or
    - x and y are direct subobjects of objects ox and oy respectively, and ox is transparently replaceable by oy. 




```cpp
struct Foo {
    int bar;
    int baz;
}

// Stack-allocate enough memory to hold a Foo with the proper alignment
alignas(Foo) unsigned char buf[sizeof(Foo)];

Foo* foo_ptr = new(&buf) Foo{1, 2}; // Construct a `Foo` object, placing it into the
// pre-allocated storage at memory address of `buf`
// and returning a pointer to that memory foo_ptr.
```

> Note: the `alignas` specifier ensures that the byte-boundaries of the buffer are the same as that of a `Foo` ([full documentation](https://en.cppreference.com/w/cpp/language/object#Alignment)).

There are many reasons outside the scope of this article as to why one would want to separate allocation from initialization, but some simple ones are:  

- It's faster to reuse pre-allocated memory than it is to allocate new memory
- When writing code for an embedded system that has memory-mapped hardware, one needs to reuse the same fixed address

Suppose we tried to access that memory via the first pointer to it with `reinterpret_cast<Foo *>(&buf)->bar`. This is actually undefined behavior, since the underlying type of &buf is `unsigned char*` and doesn't point to a `Foo` object. Even if `&buf` and `foo_ptr` point to the same address, their differing types mean that we cannot safely use them interchangeably. One way to solve this issue is with `std::launder`. Launder has an esoteric definition on [cppreference](https://en.cppreference.com/w/cpp/utility/launder):

```cpp
// Defined in <new>

template< class T >
[[nodiscard]] constexpr T* launder( T* p ) noexcept; // Since C++20
```

"Provenance fence with respect to `p`. Returns a pointer to the same memory that `p` points to, but where the referent object is assumed to have a distinct lifetime and dynamic type. Formally, given

- the pointer `p` represents the address `A` of a byte in memory
- an object `x` is located at the address `A`
- `x` is within its lifetime
- the type of `x` is the same as `T`, ignoring cv-qualifiers at every level
- every byte that would be reachable through the result is reachable through `p` (bytes are reachable through a pointer that points to an object `y` if those bytes are within the storage of an object `z` that is pointer-interconvertible with `y`, or within the immediately enclosing array of which `z` is an element). 

Then `std::launder(p)` returns a value of type `T*` that points to the object `x`. Otherwise, the behavior is undefined.

The program is ill-formed if `T` is a function type or (possibly cv-qualified) `void`."

How does this arcane definition apply in this example? Well, 

1. `&buf` points to an address A
2. an object (let's call it `foo`) is located at A
3. this object is within its lifetime (i.e. memory is allocated for it and initialized it has been initialized)
4. the type of `foo` is `Foo`
5. every byte in the returned pointer is reachable through `&buf`

In order to safely reach the memory through `&buf`, we must wrap the cast in a call to launder: `std::launder(reinterpret_cast<Foo *>(&buf))->bar`. This informs the compiler that we *can* access the memory through that pointer because a call to launder effectively treats that pointer as if it were a freshly made object (similar to a normal call to `new`). The full example is below:

```cpp
#include <new>

struct Foo {
    int bar;
    int baz;
}

// Stack-allocate enough memory to hold an int with the proper alignment
alignas(Foo) unsigned char buf[sizeof(Foo)];

Foo* foo_ptr = new(&buf) Foo{0, 1}; // Construct a `Foo` object, placing it into the
// pre-allocated storage at memory address of `buf`
// and returning a pointer to that memory foo_ptr.

foo_ptr->bar = 2; // Ok, normal access

reinterpret_cast<Foo *>(&buf)->bar = 3 // Undefined behavior

std::launder(reinterpret_cast<Foo *>(&buf))->bar = 4 // Ok, treated as a pointer to a fresh object similar 
```

You're likely wondering, "why not use `foo_ptr` directly if we already called placement `new`?" There may be scenarios where we call placement `new` without saving its return value to a fresh pointer. Consider another example from cppreference:

```cpp
struct Base {
    virtual int transmogrify();
};
 
struct Derived : Base {
    int transmogrify() override {
        new(this) Base;
        return 2;
    }
};
 
int Base::transmogrify() {
    new(this) Derived;
    return 1;
}
 
static_assert(sizeof(Derived) == sizeof(Base));
 
int main() {
    // Case 1: the new object failed to be transparently replaceable because
    // it is a base subobject but the old object is a complete object.
    Base base;
    int n = base.transmogrify();
    // int m = base.transmogrify(); // undefined Behavior
    int m = std::launder(&base)->transmogrify(); // OK
    assert(m + n == 3);
}
```

In this example, the first call to `transmogrify` changes the underlying type of `base` from `Base` to `Derived`. However, the compiler views `base` as a `Base` object and doesn't know which call to `transmogrify` to use the second time. It assumes that the "pointer" to the memory at `base` and the **actual** type of the memory it points to should be the same, leading to undefined behavior. Once again, a band-aid solution here is to use `std::launder` to tell the compiler "trust me, there really is a valid, freshly-made object at this address." Since launder doesn't affects its arguments, its return value must be stored in a variable in order to avoid the problem that **not storing the result of placement `new`** caused. What's the solution here? Unless we absolutely must use placement `new`, it's a better option to use higher-level memory-management options such as smart pointers. In cases where we *must* use placement new, a good way to forego this indirection is to save the result of placement `new` somewhere since we'll need to eventually call `std::launder` if we do not. Although `std::launder`'s use is niche, its necessity comes about when the compiler cannot 

## Resources used

- https://en.cppreference.com/w/cpp/utility/launder
- https://en.cppreference.com/w/cpp/language/new
- https://en.cppreference.com/w/cpp/types/byte
- https://en.cppreference.com/w/cpp/language/lifetime
