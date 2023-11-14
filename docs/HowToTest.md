This guide will demonstrate how to create, build, and run tests.

***Note:*** that these instructions should work on Mac and Linux systems. Windows instructions should be similar, but please add any information about working with tests on Windows via a pull request! 

***Note:*** this guide assumes you have Catch2 downloaded. 
If you haven't yet downloaded the repo, simply add a `--recursive` to the end of your `git clone`. 
If you _have_ downloaded the rest of the repo, simply initialize and update the submodules: 
```
git submodule init
git submodule update
```

## How to compile tests

First, create a build directory for tests if you don't already have one, and then enter that directory:
```
mkdir cmake-build-test
cd cmake-build-test
```

Now we need to use CMake to compile. This is a two step process.
First we tell CMake where to find the files, relative to our current directory. 
```
cmake -DCMAKE_BUILD_TYPE=Test ..
```

If that completes without errors, we can now build the files in our current directory. 
```
cmake --build .
```


That should compile the unit tests. 
Note that this process will take a while the first time as it needs to compile Catch2. It should be much faster in subsequent builds. 

### Compiling with extra source files. 
Note that if you want to compile code that depends on additional source files (i.e., additional `.cpp` files), you need to perform one additional step before compiling.
Specifically, you need to create an additional `.cmake` file. 

The `.cmake` file should have the same base name as your test's main `.cpp`, for example, `WorldGrid.cpp` would have an accompanying `WorldGrid.cmake`. 

You have free reign within this CMake file, but to add sources you will need to add lines like this (one per source file you want to add): 
```
add_source_to_target(${EXE_NAME} "source/Agents/PathAgent.cpp")
```
The `${EXE_NAME}` ***DOES NOT CHANGE***, it is a variable set in the other CMake files. You wimply need to change the string to match the path of your source file (relative to the repo's root). 

Once this file is created, compilation can proceed as normal. The CMake build system will automatically detect and execute the file you just created. 

## Running tests

Inside of your test build directory (`/cmake-build-test/` above), run the following: 
```
ctest --output-on-failure
```

This should run all the unit tests, providing something like the following: 
```bash

Test project /Users/thammina/projectsLocal/CSE491_GP_Group7/build
    Start 1: tests-unit-core-Data
1/3 Test #1: tests-unit-core-Data .............   Passed    0.01 sec
    Start 2: tests-unit-core-WorldGrid
2/3 Test #2: tests-unit-core-WorldGrid ........   Passed    0.01 sec
    Start 3: tests-unit-gp_agents-agent
3/3 Test #3: tests-unit-gp_agents-agent .......   Passed    0.01 sec

100% tests passed, 0 tests failed out of 3
```
By specifying the `--output-on-failure` flag, we should see also see the specifics of which cases failed, if any. 

If you wish to run a particular test, simply navigate to the `executable` directory inside the build folder, and then run that particular test (e.g., `tests-unit-core-Data`).
Note that it may end in `.exe` on  Windows.

## Adding new tests

To add tests for a new file, navigate to the corresponding folder in the `tests/unit` directory (not in `cmake-build-test` directory). 
For example, if we want to add a test for a new file in `core`, from the root of the repo we would navigate like so: 
```
cd tests/unit/core
```
Once in the correct directory we need to add the actual source code of the test. 
This should be done as a new `.cpp` file, which will typically have the same name as the header file in source (_e.g._, `WorldGrid.hpp` will have a new test file called `WorldGrid.cpp`). 

Once you've created that file, you can start with this skeleton code: 

```
/**
 * This file is part of the Fall 2023, CSE 491 course project.
 * @brief <TODO>
 **/

// Catch2 
#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

// Class project
// Place your includes here
// e.g., #include "core/Data.hpp"
  
TEST_CASE("NAME", "[tags]"){
}

```

This should be it!
CMake should automatically detect the file and compile (and let Austin know if that doesn't work!)
