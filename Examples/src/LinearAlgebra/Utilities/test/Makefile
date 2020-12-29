include ../../../../Makefile.inc
CPPFLAGS+=-I..

all: $(DEPEND) testMM_readers  testCooWrite testAdjacency testRotatingVector testBasicZeroFun testBasicMinFun test_mathUtils

# for multithreading
LDLIBS+=-ltbb

SRCS=testMM_readers.cpp  testCooWrite.cpp testAdjacency.cpp testRotatingVector.cpp

testMM_readers:testMM_readers.o

testMM_readers.o: testMM_readers.cpp $(INCLUDEFILES)

testCooWrite: testCooWrite.o

testCooWrite.o: testCooWrite.cpp ../COOExtractor.hpp

testAdjacency: testAdjacency.o ../Utilities.o

testAdjacency.o: testAdjacency.cpp ../COOExtractor.hpp ../Utilities.hpp

testRotatingVector.o: testRotatingVector.cpp ../RotatingVector.hpp

testBasicZeroFun.o: testBasicZeroFun.cpp ../basicZeroFun.hpp

testBasicMinFun.o: testBasicZeroFun.cpp ../basicOptimization.hpp

test_mathUtils.o: test_mathUtils.cpp ../mathUtils.hpp

../Utilities.o: ../Utilities.cpp ../Utilities.hpp

clean:
	$(RM) *.o vec_out.mtx mat_out.mtx testMM_readers testCooWrite testRotatingVector testAdjacency $(DEPEND)

$(DEPEND): $(SRCS)
	$(RM) $(DEPEND)
	for f in $(SRCS); do \
	$(CXX) $(STDFLAGS) $(CPPFLAGS) -MM $$f >> $(DEPEND); \
	done

-include $(DEPEND)
