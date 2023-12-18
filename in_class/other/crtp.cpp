#include <iostream>
#include <string>

struct VehicleBase {

};

template <typename DERIVED_T>
struct Vehicle : public VehicleBase {
  std::string name;
  std::string owner;
  virtual double GetSpeed() const { return 0; }
  DERIVED_T & Derived() { return *static_cast<DERIVED_T*>(this); }

  DERIVED_T & SetName(std::string in) { name = in; return Derived(); }
  DERIVED_T & SetOwner(std::string in) { owner = in; return Derived(); }
};

struct Quinjet : public Vehicle<Quinjet> {
  int num_missiles = 100;
  double GetSpeed() const override { return 1000000.0; }

  Quinjet & SetNumMissiles(int in) { num_missiles = in; return *this; }
};

int main()
{
  Quinjet q1;
  q1.SetName("q1").SetOwner("Avengers").SetNumMissiles(2000);
}