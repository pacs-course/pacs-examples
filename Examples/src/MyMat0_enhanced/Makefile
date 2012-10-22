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
#
exe_sources=$(filter main%.cpp,$(SRCS))
EXEC=$(exe_sources:.cpp=)

PROGRAMS=main_myMat0_move  main_myMat0_nomove

CXXFLAGS+=-O3

#========================== ORA LA DEFINIZIONE DEGLI OBIETTIVI
.phony= all clean distclean doc

.DEFAULT_GOAL = all


all: $(DEPEND) $(PROGRAMS)

main_myMat0_move:
	-rm *.o $(EXEC)
	$(MAKE)	$(EXEC)
	mv $(EXEC) main_myMat0_move

main_myMat0_nomove:
	-rm *.o $(EXEC)
	$(MAKE) $(EXEC) CPPFLAGS+=-DNOMOVE
	mv $(EXEC) main_myMat0_nomove
test:
	valgrind --tool=massif --massif-out-file=test_move.out ./main_myMat0_move
	valgrind --tool=massif --massif-out-file=test_nomove.out ./main_myMat0_nomove
	ms_print test_move.out > test_move.txt
	ms_print test_nomove.out > test_nomove.txt
clean:
	-\rm -f $(EXEC) $(OBJS)
	-\rm -r -f ./doc main_myMat0_move main_myMat0_nomove test_move.* \
         test_nomove.txt Matrix.dat
	-\rm *.out *.bak *~

distclean:
	$(MAKE) clean

doc:
	doxygen $(DOXYFILE)

$(EXEC): $(OBJS)

$(OBJS): $(SRCS)

$(DEPEND): $(SRCS)
	g++ $(CPPFLAGS) -std=c++0x -MM $(SRCS) -MF $(DEPEND)



-include $(DEPEND)




