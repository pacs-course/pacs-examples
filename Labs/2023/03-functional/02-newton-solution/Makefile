CXX      ?= g++
CXXFLAGS ?= -std=c++20
CPPFLAGS ?= -O3 -Wall -I. -Wno-conversion-null -Wno-deprecated-declarations

EXEC     = main_fcn_ptr main_lambda main_getpot_lambda 
LDFLAGS ?= 
LDLIBS  ?= 

# Flags for the muParser example.
CPPFLAG_MUPARSER ?= -I../../../../Examples/include
EXEC_MUPARSER     = main_getpot_muparser
LDFLAGS_MUPARSER ?= -L../../../../Examples/lib -Wl,-rpath=../../../../Examples/lib
LIB_MUPARSER     ?= -lmuparser

all: $(EXEC) $(EXEC_MUPARSER)

%.o: %.cpp newton.hpp
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $<

$(EXEC_MUPARSER).o: %.o: %.cpp newton.hpp muparser_fun.hpp
	$(CXX) -c $(CPPFLAGS) $(CPPFLAG_MUPARSER) $(CXXFLAGS) $<

$(EXEC): %: %.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $< $(LIBS) -o $@

$(EXEC_MUPARSER): %: %.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(LDFLAGS_MUPARSER) $< $(LIBS) $(LIB_MUPARSER) -o $@

clean:
	$(RM) *.o

distclean: clean
	$(RM) $(EXEC) $(EXEC_MUPARSER)
	$(RM) *~
