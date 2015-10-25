all:
	g++ -std=c++11 -o ../bin/stm.out test.cpp stm.cpp column.cpp cell.cpp inputEntry.cpp enumEntry.cpp
release:
	g++ -O2 -std=c++11 -o ../bin/stm.out test.cpp stm.cpp column.cpp cell.cpp inputEntry.cpp
run:
	../bin/stm.out
debug:
	g++ -std=c++11 -g -o ../bin/stm.out test.cpp stm.cpp column.cpp cell.cpp inputEntry.cpp
	gdb --tui ../bin/stm.out
valgrind:
	valgrind --leak-check=full --show-reachable=yes --track-origins=yes ../bin/stm.out

