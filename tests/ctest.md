

# How to compile for ctest

if you are using cmake in command line follow the regular steps:

```bash
mkdir build
cd build
cmake ..
make
```

Then you can run the tests with:

```bash 
ctest
```


if properly configured, you should see something like this:

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


Add a new configuration is simple:

just add
```cmake

add_test(NAME {sourcename} COMMAND {comandname})
```

for example in core:

```cmake

  add_executable(tests-unit-core-${TARGET} ${TARGET}.cpp)
  ...

  add_test(NAME tests-unit-core-${TARGET} COMMAND tests-unit-core-${TARGET})
```


for clion configuration, you can add a new configuration in the run menu: