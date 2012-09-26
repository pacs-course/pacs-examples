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
# object file for a library if needed
LIB_OBJS:=$(filter-out main%.o,$(OBJS))
# get all headers in the working directory
HEADERS=$(wildcard *.hpp)
#
exe_sources=$(filter main%.cpp,$(SRCS))
EXEC=$(exe_sources:.cpp=)
EXEC_OBJS=$(exe_sources:.cpp=.o)
#========================== ORA LA DEFINIZIONE DEGLI OBIETTIVI
.phony= all  clean

all: $(DEPEND) library $(EXEC)

library:$(STATIC_LIBFILE) $(DYNAMIC_LIBFILE)

install:
	cp ./include/*.hpp $(PACS_INC_DIR)
	cp $(STATIC_LIBFILE) $(DYNAMIC_LIBFILE) $(PACS_LIB_DIR)
clean:
	-\rm -f $(EXEC) $(OBJS)

distclean:
	$(MAKE) clean
	-\rm -r -f ./doc
	-\rm *.out *.bak *~ $(STATIC_LIBFILE) $(DYNAMIC_LIBFILE) $(DEPEND)

doc:
	doxygen $(DOXYFILE)

$(EXEC): $(EXEC_OBJS)

$(OBJS): $(SRCS)

$(STATIC_LIBFILE): $(STATIC_LIBFILE)($(LIB_OBJS))
	ar -r $(STATIC_LIBFILE) $(LIB_OBJS)

$(DYNAMIC_LIBFILE):
	make clean
	$(MAKE) CXXFLAGS=-fPIC $(LIB_OBJS)
	$(CXX) -shared $(LIB_OBJS) -o $(DYNAMIC_LIBFILE)

$(DEPEND): $(SRCS)
	g++ $(CPPFLAGS) -MM $(SRCS) > $(DEPEND)



-include $(DEPEND)




