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
#
LIBNAME=functions
DYNAMIC_LIBFILE=lib$(LIBNAME).so
STATIC_LIBFILE=lib$(LIBNAME).a

include $(MAKEFILEH_DIR)/Makefile.inc
LIBRARIES+=-L. -l$(LIBNAME)
LIBTYPE=DYNAMIC
ifeq ($(LIBTYPE),DYNAMIC)
 EXTRA_CXXFLAGS=-fPIC
 export LIBRARY=$(DYNAMIC_LIBFILE)
 LDLIBS=-ldl 
else
 EXTRA_CXXFLAGS=
 export LIBRARY=$(STATIC_LIBFILE)
 LDFLAGS+=-static
endif
export DEBUG
ifeq ($(DEBUG),yes)
  OPTFLAGS=-g -O0
else
  OPTFLAGS=-O3
endif
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
LIB_SRCS:=functions.cpp
LIB_OBJS:=$(LIB_SRCS:.cpp=.o)
LIB_HEADERS:=$(LIB_SRCS:.cpp=.hpp)
OTHER_OBJS:=
OTHER_SRCS:=
# get all headers in the working directory
HEADERS=$(wildcard *.hpp)
#
exe_sources=$(filter main%.cpp,$(SRCS))
EXEC=$(exe_sources:.cpp=)
EXEC_OBJS=$(exe_sources:.cpp=.o)
EXEC_SRCS:=$(exe_sources)
#========================== ORA LA DEFINIZIONE DEGLI OBIETTIVI
.phony= all clean distclean doc static dynamic depend install

.DEFAULT_GOAL = all

all: dynamic

depend: $(DEPEND)

static:
	$(RM) $(LIB_OBJS)
	$(MAKE) library LIBTYPE=STATIC 
	$(MAKE) exec LIBTYPE=STATIC 
dynamic:
	$(RM) $(LIB_OBJS)
	$(MAKE) library LIBTYPE=DYNAMIC
	$(MAKE) exec LIBTYPE=DYNAMIC

exec: $(EXEC)

library:  $(LIBRARY)

install:
	cp $(LIB_HEADERS) $(PACS_INC_DIR)
	cp $(STATIC_LIBFILE) $(DYNAMIC_LIBFILE) $(PACS_LIB_DIR)
clean:
	$(RM) -f $(EXEC) $(OBJS)

distclean:
	$(MAKE) clean
	$(RM) -f ./doc $(STATIC_LIBFILE) $(DYNAMIC_LIBFILE)
	$(RM) *.out *.bak *~

doc:
	doxygen $(DOXYFILE)

$(EXEC): $(EXEC_OBJS) $(OTHER_OBJS)
	$(CXX) $(EXEC_OBJS) $(OTHER_OBJS) $(LDFLAGS) $(LDLIBS) -o $(EXEC)

$(EXEC_OBJS): $(EXEC_SRCS)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(EXEC_CPPFLAGS) -c $?

$(OTHER_OBJS): $(OTHER_SRCS)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $?

$(STATIC_LIBFILE): $(STATIC_LIBFILE)($(LIB_OBJS))
	ar -r $(STATIC_LIBFILE) $(LIB_OBJS)

$(DYNAMIC_LIBFILE): $(LIB_OBJS)
	$(CXX) -shared $(OPTFLAGS) $(LIBD) $(LIB_OBJS) -o $(DYNAMIC_LIBFILE)

$(LIB_OBJS): $(LIB_SRCS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -fPIC -c $?

$(DEPEND): $(SRCS)
	$(RM) $(DEPEND)
	for f in $(SRCS); do \
	$(CXX) $(STDFLAGS) $(CPPFLAGS) -MM $$f >> $(DEPEND); \
	done

-include $(DEPEND)




