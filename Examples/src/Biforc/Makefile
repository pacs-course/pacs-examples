# get all files *.cpp
SRCS=$(wildcard *.cpp)
# get the corresponding object file
OBJS = $(SRCS:.cpp=.o)
# get all headers in the working directory
HEADERS=$(wildcard *.hpp)
#
exe_sources=$(filter main%.cpp,$(SRCS))
EXEC=$(exe_sources:.cpp=)

# =============================COMPILERS AND FLAGS
CXX=/opt/clang/bin/clang++
CPPFLAGS=-I. -I/usr/local/include/eigen3
OPTFLAGS=-g
WARNFLAGS=-Wall -Wno-deprecated
CXXFLAGS=$(OPTFLAGS) $(CPPFLAGS) $(WARNFLAGS) -std=c++11
LDFLAGS=$(OPTFLAGS)
CC=$(CXX)
#========================== ORA LA DEFINIZIONE DEGLI OBIETTIVI
.phony= all clean distclean doc

.DEFAULT_GOAL = all

DEPEND=make.dep

all: $(DEPEND) $(EXEC)

clean:
	-\rm -f $(EXEC) $(OBJS)

distclean:
	$(MAKE) clean
	-\rm -r -f ./doc
	-\rm *.out *.bak *~

depend: $(DEPEND)

doc:
	doxygen $(DOXYFILE)

$(EXEC): $(OBJS)

$(DEPEND): $(SRCS) $(HEADERS)
	@rm -f $(DEPEND)
	for i in $(SRCS); do \
	g++ $(CPPFLAGS) -MM $$i >>$(DEPEND); done

-include $(DEPEND)




