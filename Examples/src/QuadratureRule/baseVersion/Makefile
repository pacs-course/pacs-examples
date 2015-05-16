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

MAKEFILEH_DIR=../../..
include $(MAKEFILEH_DIR)/Makefile.inc
#
# You may have an include file also in the current directory
#
-include Makefile.inc
#
# Library with quadrature rules
LIBNAME=quadrules
DYNAMIC_LIBFILE=lib$(LIBNAME).so
STATIC_LIBFILE=lib$(LIBNAME).a

# Library with integrands
LIBNAME3=integrands
DYNAMIC_LIBFILE3=lib$(LIBNAME3).so
STATIC_LIBFILE3=lib$(LIBNAME3).a

EXTRA_LIBS=-L$(PACS_LIB_DIR) -lMesh1D
LIBD=$(EXTRA_LIBS)

ifeq ($(LIBTYPE),DYNAMIC)
 CXXFLAGS+=-fPIC
 export LIBRARY=$(DYNAMIC_LIBFILE)
 export LIBRARY3=$(DYNAMIC_LIBFILE3)
else
 EXTRA_CXXFLAGS=
 export LIBRARY=$(STATIC_LIBFILE)
 export LIBRARY3=$(STATIC_LIBFILE3)
endif
export DEBUG
ifeq ($(DEBUG),no)
  OPTFLAGS=-O3
  LIBRARIES=-L. -L$(PACS_LIB_DIR)  -l$(LIBNAME) -l$(LIBNAME3) $(EXTRA_LIBS)
  LDFLAGS+=-Wl,-rpath=$(PACS_LIB_DIR)
else
  OPTFLAGS=-g -O0
  LIBRARIES=-L.  -l$(LIBNAME) -l$(LIBNAME3) -L$(PACS_LIB_DIR) $(EXTRA_LIBS)
  LDFLAGS+=-Wl,-rpath=$(PACS_LIB_DIR)
endif

LDLIBS=$(LIBRARIES)

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
LIB_SRCS:=numerical_rule.cpp helperfunction.cpp numerical_integration.cpp montecarlo.cpp 
LIB_OBJS:=$(LIB_SRCS:.cpp=.o)
LIB_HEADERS:=$(LIB_SRCS:.cpp=.hpp) $(wildcard Quadrature*.hpp)
LIB3_SRCS:=integrands.cpp
LIB3_OBJS:=$(LIB3_SRCS:.cpp=.o)
LIB3_HEADERS:=$(LIB3_SRCS:.cpp=.hpp)
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
.phony= all clean distclean doc static dynamic depend install library

.DEFAULT_GOAL = all

all:
	@echo "make static makes static library"
	@echo "make dynamic makes dynamic library"
	@echo "make exec compiles executable"
	@echo "make depend just makes dependency file"
	@echo "make clean cleans all not library"
	@echo "make distclean cleans all"
	@echo "make install installs in the root directory"
	@echo "macro LIBTYPE=STATIC compiles against dynamic lib"
	@echo "macro LIBTYPE=DYNAMIC compiles against static lib"
	@echo "macro DEBUG=no deactivates debugging and uses libraries in the"
	@echo "root lib when compiling executable"
	@echo "To run executable when compiling with DEBUG=no you need to type"
	@echo "export LD_LIBRARY_PATH="$(PACS_LIB_DIR):.

depend: $(DEPEND)

static:
	-rm -f *.o
	$(MAKE) library LIBTYPE=STATIC 
dynamic:
	-rm -f *.o
	$(MAKE) library LIBTYPE=DYNAMIC

exec: $(EXEC)

library:  $(LIBRARY) $(LIBRARY3)

install:
	cp $(LIB_HEADERS) $(PACS_INC_DIR)
	mv -f $(STATIC_LIBFILE) $(DYNAMIC_LIBFILE) $(STATIC_LIBFILE2) $(DYNAMIC_LIBFILE2) $(PACS_LIB_DIR)
clean:
	$(RM) -f $(EXEC) $(OBJS)

distclean:
	$(MAKE) clean
	$(RM) -r -f ./doc *.a *.so
	$(RM) -f *.out *.bak *~ $(DEPEND)

doc:
	doxygen $(DOXYFILE)

$(EXEC): $(EXEC_OBJS) $(OTHER_OBJS)
#	$(CXX) $(EXEC_OBJS) $(OTHER_OBJS) $(LDFLAGS) $(LDLIBS) -o $(EXEC)

$(EXEC_OBJS): $(EXEC_SRCS)
#	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(EXEC_CPPFLAGS) -c $<

$(OTHER_OBJS): $(OTHER_SRCS)
#	$(CXX) $(CXXFLAGS) $(EXTRA_CXXFLAGS) $(CPPFLAGS) -c $<

$(STATIC_LIBFILE): $(STATIC_LIBFILE)($(LIB_OBJS))
#	ar -r $(STATIC_LIBFILE) $(LIB_OBJS)

$(STATIC_LIBFILE3): $(STATIC_LIBFILE3)($(LIB3_OBJS))
#	ar -r $(STATIC_LIBFILE3) $(LIB3_OBJS)

$(DYNAMIC_LIBFILE): $(LIB_OBJS)
	$(CXX) -shared $(OPTFLAGS)  $(LIB_OBJS) $(LIBD) -o $(DYNAMIC_LIBFILE)

$(DYNAMIC_LIBFILE3): $(LIB3_OBJS)
	$(CXX) -shared $(OPTFLAGS) $(LIB3_OBJS) -o $(DYNAMIC_LIBFILE3)

$(DEPEND): $(SRCS)
	$(RM) $(DEPEND)
	for f in $(SRCS); do \
	$(CXX) $(STDFLAGS) $(CPPFLAGS) -MM $$f >> $(DEPEND); \
	done



-include $(DEPEND)




