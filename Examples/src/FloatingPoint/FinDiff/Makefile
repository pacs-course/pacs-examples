###########################################################
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

MAKEFILEH_DIR=../../../
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

#========================== ORA LA DEFINIZIONE DEGLI OBIETTIVI
.PHONY: all clean distclean doc run

.DEFAUL_GOAL = all

all: $(DEPEND) main_float main_double main_extended

main_float: main_findiff.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS)  -DSINGLE_PRECISION -o $@ $<


main_double: main_findiff.cpp
	$(CXX) $(CPPFLAGS)  $(CXXFLAGS)   -o $@ $<

main_extended: main_findiff.cpp
	$(CXX) $(CPPFLAGS)  $(CXXFLAGS)  -DEXTENDED_PRECISION -o $@ $<


run:
	$(MAKE) all
	./main_float
	./main_double
	./main_extended
	gnuplot gnuplot.txt
clean:
	$(RM) -f $(EXEC) $(OBJS) main_float main_double main_extended

distclean:
	$(MAKE) clean
	$(RM) -r -f ./doc
	$(RM) *.dat *.out *.bak *~

doc:
	doxygen $(DOXYFILE)

$(EXEC): $(OBJS)

$(OBJS): $(SRCS)

$(DEPEND): $(SRCS)
	-\rm $(DEPEND)
	for f in $(SRCS); do \
	$(CXX) $(STDFLAGS) $(CPPFLAGS) -MM $$f >> $(DEPEND); \
	done



-include $(DEPEND)




