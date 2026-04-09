############################################################
#
# An example Makefile for the course on
# Advanced Programming for Scientific Computing
# It should be adapted to the different examples
#
############################################################
#
# The environment variable PACS_ROOT should be set to the
# root directory where the examples reside. In practice, it is the directory
# where this file is located. The resolution of PACS_ROOT is made in the
# Makefile.h file, where other important variables are also set.
# The only user-defined variable that must be set in this file is
# the one indicating where Makefile.h resides

MAKEFILEH_DIR=../../../
#
include $(MAKEFILEH_DIR)/Makefile.inc
#
# You may also have an include file in the current directory
#
-include Makefile.inc

#
# The general setting is as follows:
# mains are identified by main_XX.cpp
# all other files are XX.cpp
#

# Get all .cpp files
SRCS=$(wildcard *.cpp)
# Get the corresponding object files
OBJS = $(SRCS:.cpp=.o)
# Get all headers in the working directory
HEADERS=$(wildcard *.hpp)
#
exe_sources=$(filter main%.cpp,$(SRCS))
EXEC=$(exe_sources:.cpp=)

#========================== TARGET DEFINITIONS
.phony= all clean distclean doc

.DEFAULT_GOAL = all

all: $(DEPEND) $(EXEC)

clean:
	-\rm -f $(EXEC) $(OBJS)

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
	$(CXX) $(CXXFLAGS) $(STDFLAGS) $(CPPFLAGS) -MM $$f >> $(DEPEND); \
	done

-include $(DEPEND)



