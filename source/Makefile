FLAGS_version = -std=c++20
FLAGS_warn    = -Wall -Wextra -Wcast-align -Winfinite-recursion -Wnon-virtual-dtor -Wnull-dereference -Woverloaded-virtual -Wpedantic
FLAGS_all     = $(FLAGS_version) $(FLAGS_warn)

FLAGS_debug   = $(FLAGS_all) -g                     # Setup for running a debugger
FLAGS_opt     = $(FLAGS_all) -O3 -DNDEBUG           # Fastest possible executable
FLAGS_quick   = $(FLAGS_all) -DNDEBUG               # Fastest possible compilation
FLAGS_grumpy  = $(FLAGS_all) -Wconversion -Weffc++  # Some extra picky warnings

CXX = g++

# List of all available executables
EXECUTABLES = simple

default: simple

add_subdirectory(test)

# Generic rule to build default version of any executable
$(EXECUTABLES): %:
	$(CXX) $@_main.cpp $(FLAGS_all) -o $@

# Generic rule to build debug version of any executable
debug-%: 
	$(CXX) $*_main.cpp $(FLAGS_debug) -o $*

# Generic rule to build opt version of any executable
opt-%: 
	$(CXX) $*_main.cpp $(FLAGS_opt) -o $*

# Generic rule to build quick version of any executable
quick-%: 
	$(CXX) $*_main.cpp $(FLAGS_quick) -o $*


FILES_backup	= *~ *.dSYM
FILES_generated = *.o $(EXECUTABLES)

clean:
	rm -rf $(FILES_backup) $(FILES_generated)

