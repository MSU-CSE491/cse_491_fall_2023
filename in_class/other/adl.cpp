#include <iostream>
#include <string>

namespace mynamespace {
  struct MyClass { int x = 5; };

  template <typename T>
  int fun(T in) { return in.x; }
}

struct MyClass2 { int x = 10; };

  template <typename T>
  int fun(T in) { return in.x + 1; }

int main()
{
  mynamespace::MyClass obj;
  MyClass2 obj2;

  std::cout << fun(obj) << std::endl;
}