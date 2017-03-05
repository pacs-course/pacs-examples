include ../../../../Makefile.inc
CPPFLAGS+=-I..

all: $(DEPEND) testMM_readers  testCooWrite testAdjacency

SRCS=testMM_readers.cpp  testCooWrite.cpp testAdjacency.cpp

testMM_readers:testMM_readers.o

testMM_readers.o: testMM_readers.cpp $(INCLUDEFILES)

testCooWrite: testCooWrite.o

testCooWrite.o: testCooWrite.cpp ../COOExtractor.hpp

testAdjacency: testAdjacency.o ../Utilities.o

testAdjacency.o: testAdjacency.cpp ../COOExtractor.hpp ../Utilities.hpp

../Utilities.o: ../Utilities.cpp ../Utilities.hpp

clean:
	$(RM) *.o vec_out.mtx mat_out.mtx testMM_readers testCooWrite testAdjacency $(DEPEND)

$(DEPEND): $(SRCS)
	$(RM) $(DEPEND)
	for f in $(SRCS); do \
	$(CXX) $(STDFLAGS) $(CPPFLAGS) -MM $$f >> $(DEPEND); \
	done

-include $(DEPEND)
