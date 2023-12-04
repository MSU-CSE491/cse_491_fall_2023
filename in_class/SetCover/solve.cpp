#include <bitset>
#include <iostream>
#include <sstream>
#include <unordered_set>
#include <vector>

static constexpr size_t MAX_ENTRY = 1000;

struct SetCover {
  size_t U;
  std::vector<std::vector<size_t>> sets;
  std::vector<std::bitset<MAX_ENTRY>> bsets;

  // A prospective solution is a set of IDs of which subsets to use.
  using solution_t = std::vector<size_t>;

  void Load() {
    size_t N;
    std::cin >> U >> N;
    sets.resize(N);

    std::string line;
    size_t value;
    std::getline(std::cin, line); // Skip the remainder of the first line

    for (size_t i = 0; i < N; ++i) {
      std::getline(std::cin, line); // Read the whole line for each subset
      std::istringstream iss(line);

      while (iss >> value) { // Extract numbers from the line
        sets[i].push_back(value);
      }
    }

    // Fill out bitsets.
    bsets.resize(sets.size());
    for (size_t i = 0; i < sets.size(); ++i) {
      for (size_t value : sets[i]) {
        bsets[i][value] = true;
      }
    }
  }

  void Print() const {
    std::cout << U << " " << sets.size() << '\n';
    for (const auto & set : sets) {
      for (size_t value : set) std::cout << value << ' ';
      std::cout << '\n';
    }
  }

  void PrintSolution(const solution_t & sol) const {
    for (size_t id : sol) {
      std::cout << id << " :";
      for (size_t value : sets[id]) {
        std::cout << " " << value;
      }
      std::cout << '\n';
    }
  }

  // Generate the first solution of a given size.
  solution_t FirstSolution(size_t sol_size) const {
    solution_t sol(sol_size);
    for (size_t i = 0; i < sol_size; ++i) {
      sol[i] = i;
    }
    return sol;
  }

  // Move to the next solution of this size.  Return 'false' if not possible.
  bool NextSolution(solution_t & solution, size_t pos=0) const {
    // What is the roll-over value for this position?
    size_t cap = U;
    if (pos < solution.size()-1) cap = solution[pos+1];

    // Increment this position
    solution[pos]++;
    
    // If this position has hit the cap, consider next position!
    if (solution[pos] >= cap) {      
      // If we are at the last position, just return false.
      if (pos == solution.size() - 1) return false;

      // Otherwise reset it to lowest and move on.
      if (pos == 0) solution[0] = 0;
      else solution[pos] = solution[pos-1]+1;

      return NextSolution(solution, pos+1);
    }

    return true;
  }

  bool IsLegal(const solution_t & sol) {
    std::bitset<MAX_ENTRY> result;
    for (size_t id : sol) {
      result |= bsets[id];
    }

    return result.all();

    // // Scan values in any set.
    // std::unordered_set<int> full_set;
    // for (size_t id : sol) {
    //   const auto & set = sets[id];
    //   for (int value : set) full_set.insert(value);
    // }

    // // Test if all values are available.
    // for (int i = 0; i < U; ++i) {
    //   if (!full_set.count(i)) return false;
    // }
    // return true;
  }

  bool Solve(int sol_size) {
    SetCover::solution_t sol = FirstSolution(sol_size);
    do {
      if (IsLegal(sol)) {
        std::cout << "FOUND!\n";
        PrintSolution(sol);
        return true;
      }
    } while (NextSolution(sol));
    return false;
  }
};

int main(int argc, char * argv[])
{
  int sol_size = 4;
  if (argc > 1) sol_size = std::atoi(argv[1]);

  SetCover sc;
  sc.Load();

  if (!sc.Solve(sol_size)) {
    std::cout << "NOT FOUND. :-(" << std::endl;
  }
}