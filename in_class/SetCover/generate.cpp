#include <cstdlib>
#include <iostream>
#include <random>
#include <unordered_set>
#include <vector>

using subset_t = std::vector<int>;
using instance_t = std::vector<subset_t>;

void PrintSetCover(int U, const instance_t & sets) {
  std::cout << U << " " << sets.size() << '\n';
  for (const auto & set : sets) {
    for (size_t i = 0; i < set.size(); ++i) {
      if (i) std::cout << " ";
      std::cout << set[i];
    }
    std::cout << '\n';
  }
}

// Test to make sure that there IS a possible solution (i.e., all entities are available)
bool TestLegal(int U, const instance_t & sets) {
  // Scan values in any set.
  std::unordered_set<int> full_set;
  for (const auto & set : sets) {
    // if (set.size() == 0) {
    //   std::cerr << "Set size zero!" << std::endl;
    //   return false;  // No sets can be empty.
    // }
    for (int value : set) full_set.insert(value);
  }

  // Test if all values are available.
  for (int i = 0; i < U; ++i) {
    if (!full_set.count(i)) {
      std::cerr << "Not fully covered" << std::endl;
      return false;
    }
  }
  return true;
}

instance_t GenerateSetCover(int U, int N, double P, int attempt=0)
{
  if (attempt > 100) {
    std::cerr << "ERROR: Made 100 attempts; unable to generate with these parameters!" << std::endl;
    exit(1);
  }
  
  static std::random_device rd;  // Seed
  static std::mt19937 gen(rd()); // Standard mersenne_twister_engine
  static std::uniform_real_distribution<> dis(0.0, 1.0);

  instance_t sets(N);

  // Randomly fill in each set, tracking total count as we go.
  for (auto & set : sets) {
    for (size_t i = 0; i < U; ++i) {
      if (dis(gen) < P) set.push_back(i);
    }
  }

  // If this attempt isn't legal, try again.
  if (!TestLegal(U, sets)) return GenerateSetCover(U, N, P, attempt+1);
  return sets;
}

int main(int argc, char * argv[])
{
  if (argc != 4) {
    std::cerr << "This program generates instances of the set cover problem, where the minimum number\n"
	      << "of subsets must be selected that cover all of the entiteis in a universal set.\n"
	      << "\n"
	      << "Format: " << argv[0] << " [U] [N] [P]\n"
	      << " where U is the size of the universal set (total number of entities),\n"
	      << " N is the number of subsets to consider in the cover,\n"
	      << " and P is the probability of an entity being in a given subset.\n"
	      << std::endl;
    exit(1);
  }

  std::string U_str(argv[1]);  int U = std::stoi(U_str);
  std::string N_str(argv[2]);  int N = std::stoi(N_str);
  std::string P_str(argv[3]);  double P = std::stod(P_str);
  auto sets = GenerateSetCover(U, N, P);
  PrintSetCover(U, sets);
}
