all : pi run-pi

pi : pi.4th runtime.h compiler.py runtime.cc Makefile
	python compiler.py < pi.4th  > pi.cc
	g++ -O3 -std=c++17 -o pi pi.cc runtime.cc
	ci-l runtime.h compiler.py runtime.cc pi.4th Makefile

run-pi :
	time ./pi

clean:
	rm -f *.o a.out forth test pi pi.cc

#
