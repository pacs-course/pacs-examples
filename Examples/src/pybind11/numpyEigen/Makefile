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
DEBUG=no #get full optimization
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

# get all files *.cpp
SRCS=$(wildcard *.cpp)
# get the corresponding object file
OBJS = $(SRCS:.cpp=.o)
# get all headers in the working directory
HEADERS=$(wildcard *.hpp)
#
OBJS_NOPY = $(filter-out py%.o,$(OBJS))#
PY_OBJS = $(filter py%.o,$(OBJS))
exe_sources=$(filter main%.cpp,$(SRCS))
EXEC=$(exe_sources:.cpp=)
OBJS_NOEXEC=$(filter-out main%.o,$(OBJS))


MODULENAME=eigenwrapper# Must be consistsnt with what declared in the pybind wrapper
PY_INCLUDES != python3-config --includes
PY_EXT != python3-config --extension-suffix
PY_FLAGS=-fPIC -flto #flto is not strictly needed
PY_MODULE=$(MODULENAME)$(PY_EXT)
CXXFLAGS+=$(PY_FLAGS) 
# comment if you do not want to use blas and lapack 
CPPFLAGS+=-DEIGEN_USE_BLAS -DEIGEN_USE_LAPACKE
CPPFLAGS+=$(PY_INCLUDES)
LDFLAGS+=-flto 
# comment if you do not want to use blas and lapack
LDLIBS+=-lblas -llapacke

#========================== NEW THE DEFINITION OF THE TARGETS
.phony= all clean distclean doc

.DEFAULT_GOAL = all

all: $(DEPEND) pyModule $(EXEC)

pyModule: $(PY_MODULE)

$(PY_MODULE): $(OBJS_NOEXEC) $(PY_OBJS)
	$(RM) ./*.so
	$(CXX) $(CPPFLAGS) -shared $(CXXFLAGS) $(LDFLAGS) $^ $(LDLIBS) -o $(PY_MODULE)


clean:
	$(RM) -f $(EXEC) $(OBJS)

distclean:
	$(MAKE) clean
	$(RM) -f ./doc $(DEPEND)
	$(RM) *.out *.so  *.bak *~

doc:
	doxygen $(DOXYFILE)

$(EXEC): $(OBJS_NOPY)

$(OBJS): $(SRCS)

install:
	cp *.hpp $(PACS_INC_DIR)

$(DEPEND): $(SRCS)
	$(RM) $(DEPEND)
	for f in $(SRCS); do \
	$(CXX) $(STDFLAGS) $(CPPFLAGS) -MM $$f >> $(DEPEND); \
	done

-include $(DEPEND)
