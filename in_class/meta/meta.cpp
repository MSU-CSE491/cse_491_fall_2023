#include <iostream>
#include <string>
//#include <type_traits>

// static size_t NextID() {
//   static size_t next_id = 0;
//   return ++next_id;
// }

// template <typename T>
// static size_t ToID() {
//   static size_t this_id = NextID();
//   return this_id;
// }

template <typename T1, typename T2>
constexpr bool SameType(T1, T2) {
  return false;
}

template <typename T1>
constexpr bool SameType(T1, T1) {
  return true;
}

template <typename T1>
constexpr bool SameType(T1) {
  return true;
}

template <typename T1, typename T2, typename... extraTs>
constexpr bool SameType(T1 v1, T2 v2, extraTs... extra_values) {
  return SameType(v1,v2) && SameType(v1, extra_values...);
}

void TestFun(int) { std::cout << "int version!" << std::endl; }
void TestFun(bool) { std::cout << "bool version!" << std::endl; }
void TestFun(...) { std::cout << "extra version!" << std::endl; }

int main()
{
  int u = 5, v = 11, w = 22, x = 10, y = 1;
  double z = 3.14159265358979;
  std::string s("test");

  std::cout << "SameType(x,y) = " << SameType(x,y) << std::endl;
  std::cout << "SameType(x,z) = " << SameType(x,z) << std::endl;
  std::cout << "SameType(x,s) = " << SameType(x,s) << std::endl;
  std::cout << "SameType(x,1) = " << SameType(x,1) << std::endl;
  std::cout << "SameType(x) = " << SameType(x) << std::endl;
  std::cout << "SameType(u,v,w,x,y) = " << SameType(u,v,w,x,y) << std::endl;
  std::cout << "SameType(u,v,w,x,y,z) = " << SameType(u,v,w,x,y,z) << std::endl;

  // TestFun(5);
  // TestFun(true);
  // std::string str("Hello.");
  // TestFun(str);
  // TestFun("what will this do?");
  // TestFun('a');

  // std::cout << "int: " << ToID<int>() << std::endl;
  // std::cout << "int: " << ToID<int>() << std::endl;
  // std::cout << "double: " << ToID<double>() << std::endl;
  // std::cout << "std::string: " << ToID<std::string>() << std::endl;
  // std::cout << "char: " << ToID<char>() << std::endl;
  // std::cout << "int: " << ToID<int>() << std::endl;
}