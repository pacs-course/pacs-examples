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

MAKEFILEH_DIR=../../../
include $(MAKEFILEH_DIR)/Makefile.inc
#
# You may have an include file also in the current directory
#
-include Makefile.inc
LDLIBS=-lfpe
# Say no if you want full optimization
export DEBUG=yes
#CXX=g++-4.9
#CC=$(CXX)# I am not using C compiler here, simpler to set it equal to c++
STANDARD=c++11
#
LIBNAME=fpe
DYNAMIC_LIBFILE=lib$(LIBNAME).so
STATIC_LIBFILE=lib$(LIBNAME).a

LIBRARIES+=-L. -l$(LIBNAME)
LDLIBS+=$(LIBRARIES)
INCLUDES=-I. -I$(PACS_INC_DIR)

ifeq ($(LIBTYPE),DYNAMIC)
 CXXFLAGS+=-fPIC
 export LIBRARY=$(DYNAMIC_LIBFILE)
 LDFLAGS+=-dynamic 
else
 export LIBRARY=$(STATIC_LIBFILE)
# LDFLAGS+=-static # this is causing problems if you do not have static system lbraries
endif

ifeq ($(FPE_ABORT),yes)
 CPPFLAGS+=-DFPE_ABORT
endif

ifeq ($(DEBUG),yes)
  OPTFLAGS=-g
ifeq ($(LIBTYPE),DYNAMIC)
  LDFLAGS+=-Wl,-rpath=.# If debug we use the local dynamic libraries
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
	@echo "make exec [FPE_ABORT=yes] makes executable"
	@echo "make depend just makes dependency file"
	@echo "make clean cleans all but not not library"
	@echo "make distclean cleans all"
	@echo "make install installs"
	@echo "macro DEBUG=no  deactivates debugging"
	@echo "macro FPE_ABORT=yes  activates abort on FPE"
	@echo "Default debug setting:" $(DEBUG)

depend: $(DEPEND)

static:
	$(MAKE) library LIBTYPE=STATIC 
dynamic:
	$(MAKE) library LIBTYPE=DYNAMIC

exec: $(EXEC)

library:  $(LIBRARY)

install:
	cp $(LIB_HEADERS) $(PACS_INC_DIR)
	cp $(STATIC_LIBFILE) $(DYNAMIC_LIBFILE) $(PACS_LIB_DIR)
clean:
	$(RM) -f $(EXEC) $(OBJS)

distclean:
	$(MAKE) clean
	$(RM) -r -f  ./doc $(STATIC_LIBFILE) $(DYNAMIC_LIBFILE) $(DEPEND) 2>/dev/null
	$(RM) *.out *.bak *~ 2>/dev/null

doc:
	doxygen $(DOXYFILE)

$(EXEC): $(EXEC_OBJS) $(OTHER_OBJS)
#	$(CXX) $(EXEC_OBJS) $(OTHER_OBJS) $(LDFLAGS) $(LDLIBS) -o $(EXEC)

#$(EXEC_OBJS): $(EXEC_SRCS)
#	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(EXEC_CPPFLAGS) -c $?

#$(OTHER_OBJS): $(OTHER_SRCS)
#	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $?

$(STATIC_LIBFILE): $(STATIC_LIBFILE)($(LIB_OBJS))
#	ar -r $(STATIC_LIBFILE) $(LIB_OBJS)

$(DYNAMIC_LIBFILE): $(LIB_OBJS)
	$(CXX) -shared $(OPTFLAGS)  $(LIB_OBJS) $(LIBD) -o $(DYNAMIC_LIBFILE)

#$(LIB_OBJS): $(LIB_SRCS)
#	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -fPIC -c $?

$(DEPEND): $(SRCS)
	-\rm $(DEPEND)
	for f in $(SRCS); do \
	$(CXX) $(STDFLAGS) $(CPPFLAGS) -MM $$f >> $(DEPEND); \
	done

-include $(DEPEND)




