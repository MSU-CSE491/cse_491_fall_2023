#include <iostream>
#include <sstream>
#include <string>
#include <vector>

struct BaseNode {
  virtual void Print() = 0;
};

template <typename T>
struct Node : public BaseNode {
  T value;

  Node(T value) : value(value) { }

  void Print() override {
    std::cout << value << std::endl;
  }
};

struct Container {
  std::vector<BaseNode *> values;

  template <typename T>
  void AddValue(T in) {
    values.push_back( new Node<T>(in) );
  }

  void Print() {
    for (auto ptr : values) ptr->Print();
  }
};


int main()
{
  Container c;
  c.AddValue(10);
  c.AddValue("Test");
  c.AddValue('x');
  c.Print();
}