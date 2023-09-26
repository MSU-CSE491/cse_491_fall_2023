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

First, navigate to the `tests` directory. 
```
cd tests
```

Create a build directory within `tests`, if it does not already exist.
```
mkdir build
```
Note that this will likely through an error/warning if the directory already exists. 

Navigate into the `build` directory. 
```
cd build
```

Now we need to use CMake to compile. This is a two step process.
First we tell CMake where to find the files, relative to our current directory. 
```
cmake ..
```

If that completes without errors, we can now build the files in our current directory. 
```
cmake --build .
```

That should compile the unit tests. 
Note that this process will take a while the first time as it needs to compile Catch2. It should be much faster in subsequent builds. 

## Running tests

If you are on a Mac or Linux system and wish to run _all_ the unit tests, simply run 
```
./run_tests.sh
```
from _within the `build` directory_ (not just in `tests`). 

If you wish to run a particular test (or are on Windows), simply navigate to the test _within the build directory_. 
For example, if we want to run tests for `WorldGrid` (which is in core), we would navigate like so: 
```
cd unit/core
```

Once we're in the correct directory, we simply run the executable. 
On Unix systems that looks like: 
```
./tests-unit-core-WorldGrid
```
On Windows it should be similar, though it may have a `.exe` extension.

## Adding new tests

To add tests for a new file, navigate to the corresponding folder in the `tests/unit` directory (not in `tests/build`. 
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

Finally, once you have some tests coded up, you need to make one more change. 
Simply add the name of that file (including the `.cpp`!) to `targets.txt`. 
CMake uses this file to ensure we compile the appropriate files. 
