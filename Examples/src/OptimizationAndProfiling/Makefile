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
DEPEND=make.dep
EXEC=esempioExtAss

.phony+= unroll optimize debug massif leak profile

all: $(EXEC)

esempioExtAss: esempioExtAss.o rational.o
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -g $(LDFLAGS) esempioExtAss.cpp  rational.cpp -o esempioExtAss_debug
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -DNDEBUG $(LDFLAGS) $(LDLIBS) esempioExtAss.cpp  rational.cpp -o esempioExtAss_release

unroll: chrono.o
	$(CXX) -S -O0 unroll1.cpp
	mv unroll1.s unroll1_nounroll.s	
	$(CXX) -o unroll1_nounroll  -O unroll1.cpp chrono.o
	$(CXX) -S -O0 -funroll-loops  unroll1.cpp
	mv unroll1.s unroll1_unroll.s
	$(CXX) -o  unroll1_unroll -O -funroll-loops unroll1.cpp  chrono.o


clean:
	$(RM) -f *.o *.s *.log.* massif* provamassif a.out esempio1* esempio0* esempio2 esempio3 leak *.E *.S result.dat 
	$(RM) -f esempioExtAss_debug  esempioExtAss_release *.txt unroll1_nounroll unroll1_unroll

distclean: clean

optimize: chrono.o
	$(CXX) $(CPPFLAGS) $(CXXFLAGS)  -I. -O0 -o esempio0 esempio.cpp chrono.o
	$(CXX)  $(CPPFLAGS) $(CXXFLAGS)  -I. -O2 -o esempio2 esempio.cpp chrono.o
	$(CXX)  $(CPPFLAGS) $(CXXFLAGS) -I. -O3 -ffast-math  -funroll-loops -msse4 -mfpmath=sse -o esempio3 esempio.cpp chrono.o
	@echo
	@echo "**version with -O0"; ./esempio0;
	@echo
	@echo
	@echo "**version with -O2"; ./esempio2; 
	@echo
	@echo
	@echo "**version with -O3  -ffast-math -funroll-loops -msse4 -mfpmath=sse"; ./esempio3;
debug:
	$(CXX)  $(CPPFLAGS) $(CXXFLAGS)  -g -o esempio0_debug esempio.cpp chrono.cpp
	$(CXX)  $(CPPFLAGS) $(CXXFLAGS)  -g -o esempio1_debug esempio_errore.cpp chrono.cpp

leak:
	 $(CXX) $(CPPFLAGS) $(CXXFLAGS)  -Wall -g leak.cpp -o leak
	valgrind --tool=memcheck --leak-check=yes \
--demangle=yes --log-file=leak.log ./leak

profile:
	g++ -std=c++11 -pg -O esempio.cpp chrono.cpp -o esempio0
	./esempio0<dati
#	gprof --line --annotated-source ./esempio0 gmon.out >file.txt # currently not working!
	gprof --line ./esempio0 gmon.out >file.txt

massif:
	$(CXX) $(CPPFLAGS) $(CXXFLAGS)  -g provamassif.cpp -o  provamassif
	valgrind --tool=massif --demangle=yes --log-file=provamassif.log --massif-out-file=massif.out\
        ./provamassif
	ms_print massif.out > massif.txt
