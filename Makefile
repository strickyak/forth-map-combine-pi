all : forth _rcs

forth : f.h f.cc main.cc
	g++ -std=c++17 f.cc main.cc

_rcs:
	ci-l f.h f.cc main.cc Makefile

clean:
	rm -f *.o a.out forth
