CXX      ?= g++
CXXFLAGS ?= -std=c++17 -Wall -pedantic
LINK.o   := $(LINK.cc) # Use C++ linker.

CPPFLAGS += -O3 -I. -isystem../../../../Examples/include -isystem$(mkBoostInc) -isystem$(mkTbbInc)
LDLIBS   += -L$(mkBoostLib) -lboost_iostreams -lboost_system -lboost_filesystem -L$(mkTbbLib) -ltbb

SRCS=$(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)
HEADERS=$(wildcard *.hpp)

exe_sources=$(filter main%.cpp,$(SRCS))
EXEC=$(exe_sources:.cpp=)

.phony= all clean distclean

.DEFAULT_GOAL = all

all: $(EXEC)

$(EXEC): $(OBJS)

$(OBJS): $(SRCS) $(HEADERS)

clean:
	$(RM) -f $(OBJS)

distclean: clean
	$(RM) -f $(EXEC)
	$(RM) *.csv *.out *.bak *~
