#include <iostream>
#include <list>
#include <vector>

template <typename CHAR_T=char, template <typename> typename CONTAINER_T=std::vector>
struct String {
  CONTAINER_T<CHAR_T> str;

  size_t size() { return str.size(); }
};

int main()
{
  String str1;
  String<int, std::list> str2;

  str1.str.resize(10,' ');
  str2.str.resize(15,'x');

  std::cout << str1.size() << std::endl;
  std::cout << str2.size() << std::endl;
}