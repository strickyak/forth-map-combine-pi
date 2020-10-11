all : forth test demo _rcs

forth: f.h f.cc main.cc Makefile
	g++ -std=c++17 f.cc main.cc -o forth

test: f.h f.cc test.cc Makefile
	g++ -std=c++17 f.cc test.cc -o test
	./test

demo:
	./forth -v plus "+" squared "dup *" '1 dup plus squared squared squared . `should `be `256'

_rcs:
	ci-l f.h f.cc main.cc Makefile

clean:
	rm -f *.o a.out forth test
