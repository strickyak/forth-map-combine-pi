all : pi run-pi

pi : pi.4th compiler.h compiler.py runtime.cc Makefile
	python compiler.py < pi.4th  > pi.cc
	g++ -std=c++17 -o pi pi.cc runtime.cc
	ci-l compiler.h compiler.py runtime.cc pi.4th

run-pi :
	time ./pi

clean:
	rm -f *.o a.out forth test pi

#
