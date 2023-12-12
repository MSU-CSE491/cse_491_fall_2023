#include <iostream>
#include <type_traits>

struct Animal {
  std::string name;
  void PrintName() const { std::cout << name << std::endl; };
};

struct Dog : Animal {
  void Herd() { }
  void Hunt() { }
  int GetStrength() { return 10; }
};
struct Sheep : Animal {
  void Graze() { }
  int GetStrength() { return 5; }
};
struct Lion : Animal {
  void Roar() { }
  int Hunt() { return 1; }
  int GetStrength() { return 100; }
};

template <typename T>
concept CanHunt = requires(T animal) {
  { animal.Hunt() } -> std::same_as<int>;
};

template <CanHunt T>
void ManageHunt(T & hunter) {
  hunter.Hunt();
}

template <typename... Ts>
void PrintNames(Ts &... animals) {
  (std::cout << ... << animals.name) << std::endl;
  // (..., animals.PrintName());
}

template <typename... Ts>
int TotalStrength(Ts &... animals) {
  return (animals.GetStrength() + ...);
}

template <typename... Ts>
auto Multiply(Ts... values) {
  return (... * values);
}

template <typename... Ts>
int MultStrength(Ts &... animals) {
  return Multiply(animals.GetStrength() ...);
}

int main()
{
  Lion l;   l.name = "Oscar";
  Dog d;    d.name = "Spot";
  Sheep s;  s.name = "Baaaa";

  PrintNames(l, d, s, l, l);
  std::cout << "Total = " << TotalStrength(l, d, s, l, l) << std::endl;
  std::cout << "Product = " << MultStrength(l, d, s, l, l) << std::endl;

  ManageHunt(l);
  // ManageHunt(d);
  // ManageHunt(s);
}