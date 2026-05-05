############################################################
#
# An example of Makefile for the course on
# Advanced Programming for Scientific Computing
#
############################################################

MAKEFILEH_DIR=../../

-include Makefile.inc
include $(MAKEFILEH_DIR)/Makefile.inc

SRCS := $(wildcard *.cpp)
HEADERS := $(wildcard *.hpp)

MAIN_EXEC := main_myMat0
TEST_EXEC := test_myMat0
BENCH_EXEC := benchmark_myMat0

CORE_OBJS := MyMat0_util.o
MAIN_OBJS := main_myMat0.o $(CORE_OBJS)
TEST_OBJS := test_myMat0.o $(CORE_OBJS)
BENCH_OBJS := benchmark_myMat0.o $(CORE_OBJS)

TEST_LIBS := -L/usr/local/lib -lgtest -lgtest_main -pthread
BENCH_LIBS := -L/usr/local/lib -lbenchmark -lbenchmark_main -pthread
LOCAL_RPATH := -Wl,-rpath=/usr/local/lib

.PHONY: all clean distclean doc test benchmark check

.DEFAULT_GOAL := all

all: $(DEPEND) $(MAIN_EXEC) $(TEST_EXEC) $(BENCH_EXEC)

$(MAIN_EXEC): $(MAIN_OBJS)
	$(LINK.o) $(LOCAL_RPATH) $^ $(LDLIBS) -o $@

$(TEST_EXEC): $(TEST_OBJS)
	$(LINK.o) $(LOCAL_RPATH) $^ $(LDLIBS) $(TEST_LIBS) -o $@

$(BENCH_EXEC): $(BENCH_OBJS)
	$(LINK.o) $(LOCAL_RPATH) $^ $(LDLIBS) $(BENCH_LIBS) -o $@

%.o: %.cpp $(HEADERS)
	$(COMPILE.cpp) $(OUTPUT_OPTION) $<

test: $(TEST_EXEC)
	./$(TEST_EXEC)

benchmark: $(BENCH_EXEC)
	./$(BENCH_EXEC)

check: test benchmark

clean:
	$(RM) -f $(MAIN_EXEC) $(TEST_EXEC) $(BENCH_EXEC) *.o

distclean:
	$(MAKE) clean
	$(RM) -f ./doc $(DEPEND)
	$(RM) -f *.out *.bak *~ Matrix.dat benchmark.out

doc:
	doxygen $(DOXYFILE)

$(DEPEND): $(SRCS)
	$(RM) -f $(DEPEND)
	for f in $(SRCS); do \
	$(CXX) $(STDFLAGS) $(CPPFLAGS) -MM $$f >> $(DEPEND); \
	done

-include $(DEPEND)
