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

# get all files *.cpp a part pyIntersect.cpp
SRCS=$(filter-out pyIntersect.cpp,$(wildcard *.cpp))
# get the corresponding object file
OBJS = $(SRCS:.cpp=.o)
# get all headers in the working directory
HEADERS=$(wildcard *.hpp)
#
exe_sources=$(filter main%.cpp,$(SRCS))
EXEC=$(exe_sources:.cpp=)
#========================== END OF THE DEFINITION OF VARIABLES
# Variables for python bynding
PY_INCLUDE=$(shell python3 -m pybind11 --includes)
PY_LIBS=$(shell python3-config --ldflags)
PY_MODULE_NAME=pyIntersect
PY_MODULE_EXTENSION=$(shell python3-config --extension-suffix)
# Standard flags for compilation
STDFLAGS= -std=c++23 -O3 -Wall
#========================== NEW THE DEFINITION OF THE TARGETS
.phony= all clean distclean doc

.DEFAULT_GOAL = all

all: $(DEPEND) $(EXEC)

clean:
	$(RM) -f $(EXEC) $(OBJS)

distclean:
	$(MAKE) clean
	$(RM) -f ./doc $(DEPEND)
	$(RM) *.so *.out *.bak *~

doc:
	doxygen $(DOXYFILE)

$(EXEC): $(OBJS)

$(OBJS): $(SRCS)

install:
	cp *.hpp $(PACS_INC_DIR)

python:
	$(CXX) $(STDFLAGS) $(CPPFLAGS) $(PY_INCLUDE) -shared -fPIC \
		$(LDFLAGS) -o $(PY_MODULE_NAME)$(PY_MODULE_EXTENSION) pyIntersect.cpp \
		$(PY_LIBS)

$(DEPEND): $(SRCS)
	$(RM) $(DEPEND)
	for f in $(SRCS); do \
	$(CXX) $(STDFLAGS) $(CPPFLAGS) -MM $$f >> $(DEPEND); \
	done

-include $(DEPEND)
