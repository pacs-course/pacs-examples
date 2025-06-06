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
CXXFLAGS+=-std=c++20
MAKEFILEH_DIR=../../
#
include $(MAKEFILEH_DIR)/Makefile.inc

# Now the default target
all: alllibs
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
exe_sources+=$(filter test%.cpp,$(SRCS))
EXEC=$(exe_sources:.cpp=)
EXEC_OBJS=$(exe_sources:.cpp=.o)

ifeq ($(LIBTYPE),DYNAMIC)
 CXXFLAGS+=-fPIC
 LIBRARY=$(DYNAMIC_LIBFILE)
 LDFLAGS+=-dynamic 
else
 LIBRARY=$(STATIC_LIBFILE)
# LDFLAGS+=-static
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

#========================== ORA LA DEFINIZIONE DEGLI OBIETTIVI
.phony= all clean distclean doc library install test alllibs


alllibs: $(DEPEND) library

library:
	$(MAKE) static
	$(MAKE) clean
	$(MAKE) dynamic

install:
	cp $(HEADERS) $(PACS_INC_DIR)
	cp $(STATIC_LIBFILE) $(DYNAMIC_LIBFILE) $(PACS_LIB_DIR)
clean:
	 $(RM) $(EXEC) $(OBJS) $(DEPEND)

distclean:
	$(MAKE) clean
	$(RM) -r ./doc $(STATIC_LIBFILE) $(DYNAMIC_LIBFILE)
	$(RM) *.out *.bak *~

doc:
	doxygen $(DOXYFILE)

test: $(EXEC)


static: $(STATIC_LIBFILE)
	make $(STATIC_LIBFILE) LIBTYPE=STATIC

$(STATIC_LIBFILE): $(STATIC_LIBFILE)($(LIB_OBJS))
	ar -r $(STATIC_LIBFILE) $(LIB_OBJS)

dynamic: 
	make $(DYNAMIC_LIBFILE) LIBTYPE=DYNAMIC

$(DYNAMIC_LIBFILE): $(LIB_OBJS)
	$(CXX) $(CXXFLAGS) -shared $(LIB_OBJS) -o $(DYNAMIC_LIBFILE)

$(DEPEND): $(SRCS)
	$(RM) $(DEPEND)
	for f in $(SRCS); do \
	$(CXX) $(STDFLAGS) $(CPPFLAGS) -MM $$f >> $(DEPEND); \
	done

-include $(DEPEND)




