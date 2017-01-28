#g++ -g -Wall -std=c++11 -I. -I/usr/local/include/eigen3 -I../../Eigen_mm -I../include tcg.cc -o tcg

EIGENDIR=/usr/local/include/eigen3
MMDIR=../../Eigen_mm
THISINCDIR=../include
INCLUDES= -I$(THISINCDIR) -I$(MMDIR) -I$(EIGENDIR)
OPTFLAGS=-O2 -Wall
CPPFLAGS=-DNDEBUG $(INCLUDES)
CXX=g++
$(LINK.o)=g++
CXXFLAGS=-std=c++11 $(OPTFLAGS)
LDFLAGS=$(OPTFLAGS)

vpath %.h $(INCLUDES)
vpath %.hpp $(INCLUDES)

EXE=$(patsubst %.cc,%,$(wildcard *.cc))

all: $(EXE)

clean:
	-rm -f *.o $(EXE)
