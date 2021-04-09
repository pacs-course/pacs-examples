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
#
# Libraries with quadrature rules plugins
LIBNAME=AdamsRules
DYNAMIC_LIBFILE=lib$(LIBNAME).so
LIB_SRCS:=AdamsRules.cpp
LIB_OBJS:=$(LIB_SRCS:.cpp=.o)
LIB_HEADERS:=$(LIB_SRCS:.cpp=.hpp)
# Libraries with quadrature rules proxy
LIBNAME4=GaussRules
DYNAMIC_LIBFILE4=lib$(LIBNAME4).so
LIB4_SRCS:=GaussRules.cpp
LIB4_OBJS:=$(LIB4_SRCS:.cpp=.o)
LIB4_HEADERS:=$(LIB4_SRCS:.cpp=.hpp)
# Libraries with quadrature rules proxy
LIBNAME5=MontecarloRules
DYNAMIC_LIBFILE5=lib$(LIBNAME5).so
LIB5_SRCS:=MontecarloRules.cpp
LIB5_OBJS:=$(LIB5_SRCS:.cpp=.o)
LIB5_HEADERS:=$(LIB5_SRCS:.cpp=.hpp)

LIBLIB= -L$(PACS_LIB_DIR) -lquadrules -L. -lrulesFactory -lmuparser


# Library with factory
LIBNAME2=rulesFactory
DYNAMIC_LIBFILE2=lib$(LIBNAME2).so
LIB2_SRCS:=ruleFactory.cpp udfHandler.cpp
LIB2_OBJS:=$(LIB2_SRCS:.cpp=.o)
LIB2_HEADERS:=$(LIB2_SRCS:.cpp=.hpp)

# Library with integrands
LIBNAME3=udf
DYNAMIC_LIBFILE3=lib$(LIBNAME3).so
LIB3_SRCS:=udf.cpp muParserFunction.cpp
LIB3_OBJS:=$(LIB3_SRCS:.cpp=.o)
LIB3_HEADERS:=$(LIB3_SRCS:.cpp=.hpp)

# Additional libraries for the main
OTHER_LIBRARIES=-lquadrature -lMesh1D -l$(LIBNAME2) -ldl 

include $(MAKEFILEH_DIR)/Makefile.inc

LIBTYPE=DYNAMIC
CXXFLAGS+=-fPIC
export LIBRARY=$(DYNAMIC_LIBFILE)
export LIBRARY4=$(DYNAMIC_LIBFILE4)
export LIBRARY5=$(DYNAMIC_LIBFILE5)
export LIBRARY2=$(DYNAMIC_LIBFILE2)
export LIBRARY3=$(DYNAMIC_LIBFILE3)
 LDFLAGS+=

export DEBUG
ifeq ($(DEBUG),no)
  OPTFLAGS=-O3 -funroll-loops
    #LDFLAGS+= -Wl,-rpath=$(PACS_LIB_DIR)
  LDFLAGS+= -Wl,-rpath=$(PACS_LIB_DIR)

else
  OPTFLAGS=-g -O0
#  LDFLAGS+=-Wl,-rpath=$(PACS_LIB_DIR)
  LDFLAGS+=-Wl,-rpath=$(PACS_LIB_DIR)
endif
#CXX=g++-4.7
LDLIBS=-L$(PACS_LIB_DIR) -L.  $(OTHER_LIBRARIES)
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
.phony= all clean distclean doc static dynamic depend install library alllibs

.DEFAULT_GOAL = all

all:
#	@echo "make static makes static library"
	@echo "make dynamic makes dynamic library"
	@echo "make exec compiles executable"
	@echo "make depend just makes dependency file"
	@echo "make alllib makes all libraries"
	@echo "make clean cleans all not library"
	@echo "make distclean cleans all"
#	@echo "make install installs in the root directory"
#	@echo "macro LIBTYPE=static compiles against static lib"
	@echo "macro LIBTYPE=dynamic compiles against static lib"
	@echo "macro DEBUG=no deactivates debugging and uses libraries in the"
	@echo "root lib when compiling executable"
	@echo "To run executable you need to type"
	@echo "export LD_LIBRARY_PATH="$(PACS_LIB_DIR):.

alllib: dynamic

depend: $(DEPEND)

dynamic:
#	$(RM) -f *.o
	$(MAKE) library LIBTYPE=DYNAMIC

exec: $(EXEC)

library:   $(LIBRARY2) $(LIBRARY) $(LIBRARY3) $(LIBRARY4) $(LIBRARY5)

clean:
	$(RM) -f $(EXEC) $(OBJS)

distclean:
	$(MAKE) clean
	$(RM) -r -f ./doc *.a *.so
	$(RM) -f *.out *.bak *~ $(DEPEND)

doc:
	doxygen $(DOXYFILE)

$(EXEC): $(EXEC_OBJS) $(OTHER_OBJS)
	$(CXX) $(EXEC_OBJS) $(OTHER_OBJS) $(LDFLAGS) $(LDLIBS) -o $(EXEC)

$(EXEC_OBJS): $(EXEC_SRCS)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(EXEC_CPPFLAGS) -c $<

$(OTHER_OBJS): $(OTHER_SRCS)
	$(CXX) $(CXXFLAGS) $(EXTRA_CXXFLAGS) $(CPPFLAGS) -c $<

$(DYNAMIC_LIBFILE): $(LIB_OBJS)
	$(CXX) -shared $(LDFLAGS)  $(LIB_OBJS) $(LIBLIB) -o $(DYNAMIC_LIBFILE)
	
$(DYNAMIC_LIBFILE4): $(LIB4_OBJS)
	$(CXX) -shared $(LDFLAGS)  $(LIB4_OBJS) $(LIBLIB) -o $(DYNAMIC_LIBFILE4)

$(DYNAMIC_LIBFILE5): $(LIB5_OBJS)
	$(CXX) -shared $(LDFLAGS)  $(LIB5_OBJS) $(LIBLIB) -o $(DYNAMIC_LIBFILE5)

$(DYNAMIC_LIBFILE2): $(LIB2_OBJS)
	$(CXX) -shared $(LDFLAGS)  $(LIB2_OBJS) $(LIBD) -o $(DYNAMIC_LIBFILE2)

$(DYNAMIC_LIBFILE3): $(DYNAMIC_LIBFILE2) $(LIB3_OBJS)
	$(CXX) -shared $(LDFLAGS) $(LIB3_OBJS) -L$(PACS_LIB_DIR) -lmuparser $(DYNAMIC_LIBFILE2) -o $(DYNAMIC_LIBFILE3)

$(DEPEND): $(SRCS)
	$(RM) $(DEPEND)
	for f in $(SRCS); do \
	$(CXX) $(STDFLAGS) $(CPPFLAGS) -MM $$f >> $(DEPEND); \
	done



-include $(DEPEND)




