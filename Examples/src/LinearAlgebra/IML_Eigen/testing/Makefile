#g++ -g -Wall -std=c++11 -I. -I/usr/local/include/eigen3 -I../../Utilities -I../include tcg.cc -o tcg
include ../../../../Makefile.inc
-include Makefile.inc
EIGENDIR=$(EIGEN_DIR)
MMDIR=../../Utilities
THISINCDIR=../include
INCLUDES= -I$(THISINCDIR) -I$(MMDIR) -I$(EIGENDIR)
OPTFLAGS=-O2 -Wall
CPPFLAGS+=$(INCLUDES)
CXX=g++

vpath %.h $(INCLUDES)
vpath %.hpp $(INCLUDES)

EXE=$(patsubst %.cc,%,$(wildcard *.cc))

all: $(EXE)

clean:
	-rm -f *.o $(EXE)
