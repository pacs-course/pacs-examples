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
LIB_OBJS=$(filter-out main%.o test%.o,$(OBJS))
# get all headers in the working directory
HEADERS=$(wildcard *.hpp)
HEADERS+=GetPot
#
exe_sources=$(filter main%.cpp,$(SRCS))
EXEC=$(exe_sources:.cpp=)

ifeq ($(LIBTYPE),DYNAMIC)
 CXXFLAGS+=-fPIC
 export LIBRARY=$(DYNAMIC_LIBFILE)
 LDFLAGS+=-dynamic 
else
 export LIBRARY=$(STATIC_LIBFILE)
 LDFLAGS+=-static
endif

ifeq ($(DEBUG),yes)
  OPTFLAGS=-g
  LDFLAGS+=-Wl,-rpath=.# If debug we use the local dynamic libraries
else
  OPTFLAGS=-O3
  CPPFLAGS+=-DNDEBUG
endif

#========================== ORA LA DEFINIZIONE DEGLI OBIETTIVI
.phony= all clean distclean doc library install test

all: $(DEPEND) install test

library:$(STATIC_LIBFILE) $(DYNAMIC_LIBFILE)

install: library
	cp $(HEADERS) $(PACS_INC_DIR)
	cp $(STATIC_LIBFILE) $(DYNAMIC_LIBFILE) $(PACS_LIB_DIR)
clean:
	-\rm -f $(EXEC) $(OBJS) test_chrono test_cppversion

distclean:
	$(MAKE) clean
	-\rm -r -f ./doc $(STATIC_LIBFILE) $(DYNAMIC_LIBFILE)
	-\rm *.out *.bak *~

doc:
	doxygen $(DOXYFILE)

test: test_chrono test_cppversion test_toString

$(EXEC): $(OBJS)

$(OBJS): $(SRCS)

$(STATIC_LIBFILE): $(STATIC_LIBFILE)($(LIB_OBJS))
	ar -r $(STATIC_LIBFILE) $(LIB_OBJS)

$(DYNAMIC_LIBFILE):
	make clean
	$(MAKE) CXXFLAGS+=-fPIC $(LIB_OBJS)
	$(CXX) -shared $(LIB_OBJS) -o $(DYNAMIC_LIBFILE)

$(DEPEND): $(SRCS)
	-\rm $(DEPEND)
	for f in $(SRCS); do \
	$(CXX) $(STDFLAGS) $(CPPFLAGS) -MM $$f >> $(DEPEND); \
	done

-include $(DEPEND)




