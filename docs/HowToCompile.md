# How to Compile

First you'll need to clone the repo _with submodules_. 

If you haven't yet cloned the repo, run: 
```
git clone https://github.com/MSU-CSE491/cse_491_fall_2023.git --recursive
```

If you _have_ already cloned the repo, but you need to download the submodules, run the following inside the repo's directory:
```
git submodule init
git submodule update
```

Now jump to the section for your operating system.

## Compiling under Linux

If you're on a Linux machine, you first need to download some dependencies for SFML. 
Specifically, you need: 
- freetype
- x11
- xrandr
- udev
- opengl
- flac
- ogg
- vorbis
- vorbisenc
- vorbisfile
- openal
- pthread

If you're on Ubuntu or other similar Debian distro, you can run the following: 
```
sudo apt install libX-dev
```
For library X. 

Once you have all dependencies downloaded, navigate to the root of the repo and run the following to build: 
```
mkdir cmake-build-release
cd cmake-build-release
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
```
All compiled executables should be in the `/build/executables/` directory. 
Note that `Debug` can be swapped with `Release` or `Test` here.

## Compiling under MacOS

Mac shouldn't require any additional dependencies, so simply run the following, starting at the root of the repo:
```
mkdir cmake-build-release
cd cmake-build-release
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
```
All compiled executables should be in the `/build/executables/` directory. 

## Compiling under Windows with MinGW

After you have the repo and submodules downloaded, we need to ensure you have MinGW and CMake.
In a command line window, try running `g++` and `cmake`, if either give a "command not found" message, then we need to install them. 

MinGW gives us g++, and can be downloaded here: https://winlibs.com/

CMake can be downloaded from here: https://cmake.org/download/

***Note:*** You'll likely need to add both MinGW's and CMake's `bin` folder to your path (I think CMake has a `bin` directory, otherwise add whatever directory contains the executable). 
To add a directory to your path, follow these instructions: https://www.architectryan.com/2018/03/17/add-to-the-path-on-windows-10/
Note that you may run into issues if the directories you're adding have spaces in their paths (e.g., "C:\Program Files\..." was giving me issues). If you run into this, I'd recommend creating a new directory on your `C:\` drive, like `C:\bin\` and then add CMake and MinGW as subdirectories there. 

Once you have CMake and MinGW working in your terminal (note you'll have to restart cmd/VSCode/whatever to get the path changes to take effect), run the following from the root of your repo:
```
mkdir build-build-release
cd build-build-release
cmake -DCMAKE_BUILD_TYPE=Release -G "MinGW Makefiles" ..
cmake --build .
```

All compiled executables should be in the `/build/executables/` directory. 

Note that if you tried to build using CMake before, it likely tried to use MSVC as a compiler. If so, just wipe the build folder and start fresh. 

## How to compile in debug mode

First, it is recommended you build debug and release modes in separate directories. 
To follow CLion's lead, you can create both `cmake-build-debug` and `cmake-build-release`.
This will prevent you from needing to recompile Catch and SFML when you switch between debug and release!

To compile in debug mode, change the build type to debug via `-DCMAKE_BUILD_TYPE=Debug` in the _first_ cmake command. 

This would normally look like: 
```
cmake -DCMAKE_BUILD_TYPE=Debug ..
```
Unless you're on Windows, which would be:

```
cmake -DCMAKE_BUILD_TYPE=Debug -G "MinGW Makefiles" ..
```

You can then build like normal. 

Note that you can also replace `Debug` with `Test` to run only the unit tests.
