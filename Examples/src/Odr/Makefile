include ../../Makefile.inc
all:main main2
main: main.o fun.o fun2.o
	@echo " linking fun.o first"
	$(CXX) -O2 -pedantic -Wall -o main fun.o fun2.o main.o
main2: main.o fun.o fun2.o
	@echo "linking fun2 first"
	$(CXX) -O2 -pedantic -Wall -o main2 fun2.o fun.o main.o
clean:
	$(RM) *.o main main2
