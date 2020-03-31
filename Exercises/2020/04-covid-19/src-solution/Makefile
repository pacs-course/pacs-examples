CXX      ?= g++
CXXFLAGS ?= -std=c++17
CPPFLAGS ?= -O3 -Wall -pedantic -I. -isystem../../../../Examples/include \
            -isystem$(mkBoostInc) -isystem$(mkTbbInc)

EXEC     = main
LDFLAGS ?= -L$(mkBoostLib) -L$(mkTbbLib)
LIBS    ?= -lboost_iostreams -lboost_system -lboost_filesystem -ltbb

SRCS=$(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)

all: $(EXEC)

%.o: %.cpp Parameters.hpp | %.hpp
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $<

$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ $(LIBS) -o $@

clean:
	$(RM) *.o

distclean: clean
	$(RM) $(EXEC)
	$(RM) *.csv *.out *.bak *~
