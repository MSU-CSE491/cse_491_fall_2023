FLAGS_warn    = -Wall -Wextra -Wcast-align -Winfinite-recursion -Wnon-virtual-dtor -Wnull-dereference -Woverloaded-virtual -pedantic

default: simple

simple:
	g++ simple_main.cpp -std=c++20 $(FLAGS_warn) -o game

FILES_backup	= *~ *.dSYM
FILES_generated = *.o game

clean:
	rm -rf $(FILES_backup) $(FILES_generated)
