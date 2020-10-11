all : forth demo _rcs

forth: f.h f.cc main.cc Makefile
	g++ -std=c++17 f.cc main.cc -o forth

demo:
	./forth plus '+' squared 'dup *' "1 dup plus squared squared squared ."

_rcs:
	ci-l f.h f.cc main.cc Makefile

clean:
	rm -f *.o a.out forth
