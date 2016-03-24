FILES :=            \
    Collatz_single.c++

CXX        := g++
CXXFLAGS   := -pedantic -Wall
LDFLAGS    := -lgtest -lgtest_main -pthread
GCOV       := gcov-4.8
GCOVFLAGS  := -fprofile-arcs -ftest-coverage
GPROF      := gprof
GPROFFLAGS := -pg
VALGRIND   := valgrind

RunCollatz: Collatz_single.c++
	$(CXX) $(CXXFLAGS) $(GPROFFLAGS) Collatz_single.c++ -o RunCollatz

clean:
	rm -f *.gcda
	rm -f *.gcno
	rm -f *.gcov
	rm -f RunCollatz
	rm -f RunCollatz.tmp
	rm -f TestCollatz
	rm -f TestCollatz.tmp

