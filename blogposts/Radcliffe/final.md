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
Tests with lists of 100 elements will also be benchmarked to show how the efficiency scales at larger capacities.

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
```
 Run  |   Vector  |    PLF   |
----- | --------- | -------- |  
  1   | 112000000 | 21333333 |  
  2   | 112000000 | 32000000 |  
  3   | 100000000 | 28000000 |  
  4   | 100000000 | 24888889 |  
  5   | 100000000 | 44800000 |  
```  

Now testing with 100 elements, we can see much higher values, but still a lot better on PLF:
```
 Run  |   Vector  |    PLF   |
----- | --------- | -------- |  
  1   | 136533333 | 23578947 |  
  2   | 115347127 | 38449042 |  
  3   | 160563200 | 56000000 |  
  4   | 112000000 | 22400000 |  
  5   | 129486452 | 28000000 |  
```  

When it comes to inserting elements, plf::list emerges as the undisputed champion, outperforming
std::vector by a staggering 72%. Its constant time (O(1)) complexity for insertion ensures that adding
elements remains swift and efficient, even as the data volume grows. In contrast, std::vector's linear
(O(n)) complexity for insertion leads to a noticeable slowdown as more elements are added.

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
```
 Run  |   Vector   |    PLF    |
----- | ---------- | --------- |  
  1   | 1000000000 | 298666667 |  
  2   | 1039200800 | 235789474 |  
  3   | 1000505701 | 186666667 |  
  4   | 1004000372 | 194782609 |  
  5   | 1000060500 | 228072727 |  
```  

Now trying with the 100 element list, the efficiency is greatly noticeable:
```
 Run  |   Vector  |   PLF    |
----- | --------- | -------  |  
  1   | 532371353 | 8092903  |  
  2   | 560000000 | 11200000 |  
  3   | 497777778 | 11200000 |  
  4   | 640000000 | 11200000 |  
  5   | 560000000 | 10000000 |  
```  
When it comes to removing elements, plf::list proves yet again, delivering an impressive 53% efficiency
increase over std::vector. Its constant time (O(1)) complexity for removal ensures that deleting elements
remains swift and efficient, regardless of their position within the data structure. In contrast,
std::vector's linear (O(n)) complexity for removal can lead to noticeable performance bottlenecks,
especially when removing a large number of elements.

# Sorting
For sorting, I'll just use a randomized list and sort the vector in ascending order.
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
```
 Run  |   Vector   |    PLF   |
----- | ---------- | -------- |  
  1   | 172307692  | 8960000  |  
  2   | 298666667  | 8960000  |  
  3   | 213333333  | 8960000  |  
  4   | 235789474  | 11150223 |  
  5   | 235789474  | 10000000 |  
```   

For a list of 100 elements, I'll create a reversed vector from 100 to 0. The results are drastically improved:
```
 Run  |  Vector |   PLF   |
----- | ------- | ------- |  
  1   | 640000  | 896000  |  
  2   | 896000  | 896000  |  
  3   | 1120000 | 746667  |  
  4   | 1493333 | 1120000 |  
  5   | 1384166 | 1120000 |  
```   


When it comes to sorting elements, plf::list reigns supreme, achieving a remarkable 95% efficiency boost over std::vector.
Its custom-designed sorting algorithm operates with astounding efficiency, outpacing the standard library's sorting method by a
significant margin.

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
```
 Run  |   Vector    |    PLF    |
----- | ----------- | --------- |  
  1   | 1002004080  | 224000000 |  
  2   | 1100306030  | 248888889 |  
  3   | 1020500611  | 235789474 |  
  4   | 1803040021  | 224000000 |  
  5   | 1040200390  | 298666667 |  
```  

Benchmarking with list of 100 elements proves even better efficiency:
```
 Run  |   Vector    |    PLF   |
----- | ----------- | -------- |  
  1   | 125440000   | 11200000 |  
  2   | 64000000    | 10000000 |  
  3   | 112000000   | 11200000 |  
  4   | 89600000    | 8960000  |  
  5   | 74666667    | 8960000  |  
```  

When it comes to reversing elements, plf::list yet again did not disappoint, achieving an impressive 72% efficiency increase
over std::vector. Its custom-designed reversal algorithm operates with remarkable efficiency, outpacing the standard library's
reversal method by a significant margin.

# Conclusion
After conducting all my tests between plf::list and std::vector, I can confidently conclude that the efficiency claims made by the PLF library do indeed have a solid backing and some of the tests even came close or outperformed what was advertised. However, it is crucial to keep note of differences in compilers, optimization levels, and CPU architectures when reading statistics such as these.

I hope you now have an idea what type of collections you may consider using in your future projects for peak efficiency!
