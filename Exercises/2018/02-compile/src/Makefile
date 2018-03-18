
CXXFLAGS = -std=c++11 -O2 -fPIC
CPPFLAGS = -DNDEBUG -I$(mkSuitesparseInc) -I$(mkEigenInc) -I../include
LDFLAGS = -L. -L$(mkSuitesparseLib)
LIBS = -lumfpack -llinearalgebra

.PHONY : all clean distclean

all : test_all

test_all : test_all.o liblinearalgebra.so
	$(CXX) $(LDFLAGS) test_all.o -o test_all $(LIBS)


test_all.o : test_all.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c test_all.cpp

liblinearalgebra.so : Utilities.o
	$(CXX) $(LDFLAGS) -shared -Wl,-soname,liblinearalgebra.so \
	Utilities.o -o liblinearalgebra.so

Utilities.o : Utilities.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c Utilities.cpp

clean :
	$(RM) *.o 

distclean : clean
	$(RM) liblinearalgebra.so test_all

