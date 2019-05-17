

all : main

libconcrete1.so : concrete1.o
	$(CXX) -shared -Wl,-soname,libconcrete1.so.1 -o libconcrete1.so.1.0 concrete1.o
	ln -fs libconcrete1.so.1.0 libconcrete1.so.1
	ln -fs libconcrete1.so.1.0 libconcrete1.so

concrete1.o : concrete1.cpp
	$(CXX) -c -fPIC concrete1.cpp

libconcrete2.so : concrete2.o
	$(CXX) -shared -Wl,-soname,libconcrete2.so.1 -o libconcrete2.so.1.0 concrete2.o
	ln -fs libconcrete2.so.1.0 libconcrete2.so.1
	ln -fs libconcrete2.so.1.0 libconcrete2.so


concrete2.o : concrete2.cpp
	$(CXX) -c -fPIC concrete2.cpp

main : main.o libconcrete2.so libconcrete1.so
	$(CXX) main.o -o main -L. -lconcrete1 -lconcrete2


clean :
	$(RM) *.o

distclean : clean
	$(RM) libconcrete*.so libconcrete*.so.* main

