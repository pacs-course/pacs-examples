############################################################
#
# An example of Makefile for the course on 
# Advanced Programming for Scientific Computing
# It should be modified for adapting it to the various examples
#
############################################################
#
# The environmental variable PACS_ROOT should be set to the
# root directory where the examples reside. In practice, the directory
# where this file is found. The resolution of PACS_ROOT is made in the
# Makefile.h file, where other important variables are also set.
# The only user defined variable that must be set in this file is
# the one indicating where Makefile.h resides

MAKEFILEH_DIR=../../
#
include $(MAKEFILEH_DIR)/Makefile.inc
#
# You may have an include file also in the current directory
#
-include Makefile.inc

#
# The general setting is as follows:
# mains are identified bt main_XX.cpp
# all other files are XX.cpp
#

# get all files *.cpp
SRCS=$(wildcard *.cpp)
# get the corresponding object file
OBJS = $(SRCS:.cpp=.o)
# get all headers in the working directory
HEADERS=$(wildcard *.hpp)

OPTFLAGS = -g
# -c       compile only, generate corresponding .o files
# -g       generate code suitable for the debugger
# -p       generate code suitable for the profiler
# -OX      attemp code optimizations at level X

#
LDFLAGS+=$(OPTFLAGS)
LDLIBS=-L$(PACS_LIB_DIR) -Wl,-rpath,$(PACS_LIB_DIR) -lpacs
CXXFLAGS+=$(OPTFLAGS)
DEPEND=make.dep
EXEC=esempioExtAss

.phony+= unroll optimize debug massif leak profile aliasing

all:
	@echo "Possible Targets:"
	@echo "esempioExtAss Use of some extended assert facilities"
	@echo "aliasing An example of argument aliasing"
	@echo "optimize Compare an optimized and an not optimized version"
	@echo "leak Find a memory leak in a program with valgrind"
	@echo "profile Program profiling with gprof"
	@echo "massif Memory usage using valgrind"
	@echo "debug Example for debugging"
	@echo "coverage Program coverage with gcov"
	@echo "lcoverage Program coverage with lcov"

esempioExtAss:
	$(CXX) $(CPPFLAGS) $(LDFLAGS) $(CXXFLAGS) -g  esempioExtAss.cpp  rational.cpp -o esempioExtAss_debug
	$(CXX) $(CPPFLAGS) $(LDFLAGS) $(CXXFLAGS)  -O2 -DNDEBUG esempioExtAss.cpp  rational.cpp -o esempioExtAss_release

aliasing: aliasing.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) -O3 -o aliasing aliasing.cpp
clean:
	$(RM)  *.o *.s *.log.* massif* provamassif a.out esempio1* esempio0* esempio2 esempio3 leak *.E *.S result.dat aliasing *.gcov *.gcda *.gcno esempio
	$(RM)  esempioExtAss_debug  esempioExtAss_release *.txt unroll1_nounroll unroll1_unroll coverage.info
	$(RM) -r html/


distclean: clean
	$(RM) core *.log gmon.out

optimize:
	$(CXX) $(CPPFLAGS)  $(LDFLAGS) -O0 -o esempio0 esempio.cpp $(LDFLAGS) $(LDLIBS)
	$(CXX)  $(CPPFLAGS)  $(LDFLAGS) -O2 -o esempio2 esempio.cpp $(LDFLAGS) $(LDLIBS)
	$(CXX)  $(CPPFLAGS) $(LDFLAGS) -O3 -ffast-math  -funroll-loops -msse4 -mfpmath=sse -o esempio3 esempio.cpp  $(LDLIBS)
	@echo
	@echo "**version with -O0"; ./esempio0;
	@echo
	@echo
	@echo "**version with -O2"; ./esempio2; 
	@echo
	@echo
	@echo "**version with -O3  -ffast-math -funroll-loops -msse4 -mfpmath=sse"; ./esempio3;
debug:
	$(CXX)  $(CPPFLAGS) $(CXXFLAGS)  -o esempio0_debug esempio.cpp $(LDFLAGS) $(LDLIBS)
	$(CXX)  $(CPPFLAGS) $(CXXFLAGS)   -o esempio1_debug esempio_errore.cpp $(LDFLAGS) $(LDLIBS)

leak:
	 $(CXX) $(CPPFLAGS) $(CXXFLAGS)  -Wall -g leak.cpp -o leak
	valgrind --tool=memcheck --leak-check=yes \
--demangle=yes --log-file=leak.log ./leak

profile:
#	g++ -std=c++11 -pg -O esempio.cpp chrono.cpp -o esempio0
	echo " With no optimization"
	g++ -std=c++17 $(CPPFLAGS) -pg  -O0 esempio.cpp  -o esempio0 $(LDLIBS)
	./esempio0 -i dati
	gprof --line  --demangle ./esempio0 gmon.out >Profiling_nopt.txt
	echo " With optimization"
	g++ -std=c++17 -pg -O2 $(CPPFLAGS)  esempio.cpp -o esempio0 $(LDLIBS)
	./esempio0 -i dati
	gprof --line --demangle ./esempio0 gmon.out >Profiling_opt.txt
#	gprof --line --demangle --annotated-source ./esempio0 gmon.out >file.txt # works only with -g

massif:
	$(CXX) $(CPPFLAGS) $(CXXFLAGS)  -g provamassif.cpp -o  provamassif
	valgrind --tool=massif --demangle=yes --log-file=provamassif.log --massif-out-file=massif.out\
        ./provamassif
	ms_print massif.out > massif.txt

coverage:
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -g -fprofile-arcs -ftest-coverage esempio.cpp -o esempio $(LDFLAGS) $(LDLIBS)
	./esempio -i dati
	gcov  --demangled-names ./esempio > summary.txt

lcoverage:
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -g -fprofile-arcs -ftest-coverage esempio.cpp -o esempio $(LDFLAGS) $(LDLIBS)
	./esempio -i dati
	lcov --capture --directory . --output-file coverage.info
	genhtml coverage.info --demangle-cpp --output-directory html


