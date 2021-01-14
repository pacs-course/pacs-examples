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
# You may have an include file also in the current directory
#
-include Makefile.inc

# Say no if you want full optimization
export DEBUG=yes
#CXX=g++-4.9
CC=$(CXX)# I am not using C compiler here, simpler to set it equal to c++
STANDARD=c++17
#
LIBNAME=bcCond
DYNAMIC_LIBFILE=lib$(LIBNAME).so
STATIC_LIBFILE=lib$(LIBNAME).a

include $(MAKEFILEH_DIR)/Makefile.inc
LIBRARIES+=-L. -l$(LIBNAME) -L$(PACS_LIB_DIR) -lpacs -lmuparserx
LDLIBS+=$(LIBRARIES)
CPPFLAGS+=-I. -I$(PACS_INC_DIR)/muparserx

ifeq ($(LIBTYPE),DYNAMIC)
 CXXFLAGS+=-fPIC
  LIBRARY=$(DYNAMIC_LIBFILE)
 LDFLAGS+=-dynamic 
else
  LIBRARY=$(STATIC_LIBFILE)
 LDFLAGS+=-static
endif

ifeq ($(DEBUG),yes)
  OPTFLAGS=-g
  ifeq ($(LIBTYPE),DYNAMIC)
   LDFLAGS+=-Wl,-rpath . # If debug we use the local dynamic libraries
  endif
else
  OPTFLAGS=-O3
  CPPFLAGS+=-DNDEBUG
endif
ifeq ($(FPEABORT),yes)
 EXEC_CPPFLAGS=-DFPE_ABORT
else
 EXEC_CPPFLAGS=
endif

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
LIB_SRCS=$(filter-out main%.cpp,$(SRCS))
LIB_OBJS=$(LIB_SRCS:.cpp=.o)
LIB_HEADERS=$(LIB_SRCS:.cpp=.hpp)
OTHER_OBJS=
OTHER_SRCS=
# get all headers in the working directory
HEADERS=$(wildcard *.hpp)
#
exe_sources=$(filter main%.cpp,$(SRCS))
EXEC=$(exe_sources:.cpp=)
EXEC_OBJS=$(exe_sources:.cpp=.o)
EXEC_SRCS:=$(exe_sources)

#========================== ORA LA DEFINIZIONE DEGLI OBIETTIVI
.PHONY: all clean distclean doc static dynamic depend install exec library

.DEFAULT_GOAL = all

all:
	@echo "make static makes static library and executable"
	@echo "make dynamic makes dynamic library and executable"
	@echo "make depend just makes dependency file"
	@echo "make clean cleans all but not not library"
	@echo "make distclean cleans all"
	@echo "make install installs"
	@echo "macro DEBUG=no  deactivates debugging"
	@echo "macro FPEABORT=yes  activates abort on FPE"
	@echo "Default debug setting:" $(DEBUG)

depend: $(DEPEND)

targets: library exec

static:
	make targets LIBTYPE=STATIC 
dynamic:
	make targets LIBTYPE=DYNAMIC

exec: $(EXEC)

library:  $(LIBRARY)

install:
	cp $(LIB_HEADERS) $(PACS_INC_DIR)
	cp $(STATIC_LIBFILE) $(DYNAMIC_LIBFILE) $(PACS_LIB_DIR)
clean:
	-\rm -f $(EXEC) $(OBJS)

distclean:
	$(MAKE) clean
	-\rm -r -f ./doc $(STATIC_LIBFILE) $(DYNAMIC_LIBFILE) $(DEPEND) 2>/dev/null
	-\rm *.out *.bak *~ 2>/dev/null

doc:
	doxygen $(DOXYFILE)

main_testbcContainer: $(LIBRARY) main_testbcContainer.o
	$(CXX) $(CXXFLAGS) main_testbcContainer.o $(LDFLAGS) $(LDLIBS) -o $@

main_testbcContainer.o: main_testbcContainer.cpp

$(STATIC_LIBFILE): $(STATIC_LIBFILE)($(LIB_OBJS))
#	ar -r $(STATIC_LIBFILE) $(LIB_OBJS)

$(DYNAMIC_LIBFILE): $(LIB_OBJS)
	$(CXX) -shared $(OPTFLAGS) $(LIBD) $(LIB_OBJS) -o $(DYNAMIC_LIBFILE)

#$(LIB_OBJS): $(LIB_SRCS)
#	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -fPIC -c $?

$(DEPEND): $(SRCS)
	-\rm $(DEPEND)
	for f in $(SRCS); do \
	$(CXX) $(STDFLAGS) $(CPPFLAGS) -MM $$f >> $(DEPEND); \
	done

-include $(DEPEND)




