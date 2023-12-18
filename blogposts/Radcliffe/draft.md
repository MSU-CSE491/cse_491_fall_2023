# Are PLF Library Collections More Efficient Than the STL?
### Author: Dominick Radcliffe

Collections are a crucial aspect of software development and they hold the data we need. Therefore, it is vital that these data structures be efficient and effective. The C++ Standard Template Library (STL) most notably houses the [vector](https://cplusplus.com/reference/vector/vector/) data structure.

![](https://i.stack.imgur.com/TcR3l.png)

## The Premise
The PLF library makes some bold claims with its list implementation by calling it a drop-in replacement for std::vector.

Top 4 commonly used operations and their optimizations:
- [293% faster insertion operations](#insertion)
- [57% faster erasure](#erasure)
- [77% faster sorting](#sorting)
- [70% faster reversal](#reversal)

&nbsp;

# The Setup
Creating a test environment with some simple test cases to collect benchmarks. Using GCC 6.3.0 as the compiler and [Google Benchmark](https://github.com/google/benchmark) to log run efficiency and times.  
  
  Benchmarking will use CPU cycles to determine efficiency and each test will be run 5 times to calculate an average score.
  
  All tests will be setup in the style of unit tests to run benchmarks. Passing in a state to a range based for loop will now time that snippet:
  ```cpp
  static void BM_SomeFunction(benchmark::State& state) {
    for (auto _ : state) {
        // This code gets timed
        SomeFunction();
    }
}
  ```  
  Then you must register this function to run it:
  ```cpp
  // Register the function as a benchmark
BENCHMARK(BM_SomeFunction);
// Run the benchmark
BENCHMARK_MAIN();
  ```

&nbsp;

# Insertion
Benchmarking a simple `push_back` operation on each of the lists is a proper starting point:
```cpp
static void BM_VectorInsert(benchmark::State& state) {
    std::vector<int> list = {1, 2, 3, 4};
    for (auto _ : state) {
        list.push_back(1);
    }
}

static void BM_PLFInsert(benchmark::State& state) {
    plf::list<int> list = {1, 2, 3, 4};
    for (auto _ : state) {
        list.push_back(1);
    }
}
```  
The results are as follows:  
Vec: 112000000
PLF: 21333333  
Vec: 112000000
PLF: 32000000  
Vec: 100000000
PLF: 28000000  
Vec: 100000000
PLF: 24888889  
Vec: 100000000
PLF: 44800000  
  
This gives us a 72% average increase in efficiency, sweet!

# Erasure
For removal, just removing the last element in the list should suffice for a meaningful test.
```cpp
static void BM_VectorErase(benchmark::State& state) {
    std::vector<int> list = {1, 2, 3, 4};

    for (auto _ : state) {
        list.erase(list.begin() + 3);
    }
}

static void BM_PLFErase(benchmark::State& state) {
    plf::list<int> list = {1, 2, 3, 4};

    for (auto _ : state) {
        list.remove(4);
    }
}
```
The results are as follows:  
Vec: 1000000000
PLF: 298666667  
Vec: 1039200800
PLF: 235789474  
Vec: 1000505701
PLF: 186666667  
Vec: 1004000372
PLF: 194782609  
Vec: 1000060500
PLF: 228072727  
  
With that, we have an approximate 53% increase, a lot closer estimate to what is advertised unlike insertion.

# Sorting  
For sorting I'll just use a randomized list and sort the vector in ascending order.  
```cpp
static void BM_VectorSort(benchmark::State& state) {
    std::vector<int> list = {4, 2, 1, 3};

    for (auto _ : state) {
        std::sort(list.begin(), list.end());
    }
}

static void BM_PLFSort(benchmark::State& state) {
    plf::list<int> list = {4, 2, 1, 3};

    for (auto _ : state) {
        list.sort();
    }
}
```  
The results are as follows:  
Vec: 172307692
PLF: 8960000  
Vec: 298666667
PLF: 8960000  
Vec: 213333333
PLF: 8960000  
Vec: 235789474
PLF: 11150223  
Vec: 235789474
PLF: 10000000  
  
  That's about a 95% increase which is great! The difference in using the standard library sort versus their built in method is quite significant.

&nbsp;

# Reversal  
Reversing a list is the final topic I am going to cover, using our trusty code we have seen:  
```cpp
static void BM_VectorRev(benchmark::State& state) {
    std::vector<int> list = {1, 2, 3, 4};

    for (auto _ : state) {
        std::reverse(list.begin(), list.end());
    }
}

static void BM_PLFRev(benchmark::State& state) {
    plf::list<int> list = {1, 2, 3, 4};

    for (auto _ : state) {
        list.reverse();
    }
}
```  
The results are as follows:  
Vec: 1002004080
PLF: 224000000  
Vec: 1100306030
PLF: 248888889  
Vec: 1020500611
PLF: 235789474  
Vec: 1803040021
PLF: 224000000  
Vec: 1040200390
PLF: 298666667

That leaves us with an approximate 72% increase which is pretty spot on with what the PLF library claims.

# Conclusion  
After conducting all my tests between plf::list and std::vector, I can confidently conclude that the efficiency claims made by the PLF library do indeed have a solid backing and some of the tests even came close or outperformed what was advertised. However, it is crucial to keep note of differences in compilers, optimization levels, and CPU architectures when reading statistics such as these.  
  
  I hope you now have an idea what type of collections you may consider using in your future projects for peak efficiency!
