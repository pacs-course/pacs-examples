CXX=g++
#CXXFLAGS=-O2 -std=c++11 -Wall
CXXFLAGS=-g -std=c++11 -Wall
CPPFLAGS=-I/usr/local/include/eigen3 -I..
LDFLAGS=-O2
INCLUDEFILES=../MM_readers.hpp
LINK.o = $(CXX) $(LDFLAGS) $(TARGET_ARCH)

all: testMM_readers  testCooWrite testAdjacency

testMM_readers:testMM_readers.o

testMM_readers.o: testMM_readers.cpp $(INCLUDEFILES)

testCooWrite: testCooWrite.o

testCooWrite.o: testCooWrite.cpp ../COOExtractor.hpp

testAdjacency: testAdjacency.o ../Utilities.o

testAdjacency.o: testAdjacency.cpp ../COOExtractor.hpp ../Utilities.hpp

../Utilities.o: ../Utilities.cpp ../Utilities.hpp

clean:
	$(RM) *.o vec_out.mtx mat_out.mtx testMM_readers testCooWrite testAdjacency
