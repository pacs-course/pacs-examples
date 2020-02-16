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
# This is optional. If not present is not an error
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

#========================== NEW THE DEFINITION OF THE TARGETS
.phony= all clean distclean doc

.DEFAULT_GOAL = all

all: $(DEPEND) $(EXEC)

clean:
	$(RM) -f $(EXEC) $(OBJS)

distclean:
	$(MAKE) clean
	$(RM) -f ./doc $(DEPEND)
	$(RM) *.out *.bak *~

doc:
	doxygen $(DOXYFILE)

$(EXEC): $(OBJS)

$(OBJS): $(SRCS)

$(DEPEND): $(SRCS)
	$(RM) $(DEPEND)
	for f in $(SRCS); do \
	$(CXX) $(STDFLAGS) $(CPPFLAGS) -MM $$f >> $(DEPEND); \
	done

%.pdf:%.tex
	$(RM) $*.aux $*.log $*.vrb $*.out
	pdflatex $<
	pdflatex $<

-include $(DEPEND)




