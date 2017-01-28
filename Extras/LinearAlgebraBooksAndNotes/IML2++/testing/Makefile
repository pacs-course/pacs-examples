SPARSELIB=../../Sparselib++/sparselib_1_7
MVLIB=../../Sparselib++/sparselib_1_7/mv
MM=../../MatrixMarket_io

INCLUDES= ../include $(SPARSELIB)/include $(MVLIB)/include $(MM)/include 
LIBS=$(SPARSELIB)/lib $(MVLIB)/lib $(MM)/lib

vpath %.h $(INCLUDES)
vpath %.a $(LIBS)

CC=g++

CPPFLAGS=-I../include -I$(SPARSELIB)/include -I$(MVLIB)/include -I$(MM)/include -DMV_VECTOR_BOUNDS_CHECK -'DCOMPLEX=std::complex<double>'

CXXFLAGS=-g -Wall

LDLIBS=-L$(SPARSELIB)/lib -L$(MVLIB)/lib -L$(MM)/lib -lMMio -lmv -lsparse -lspblas -lmv -lmv 

#EXE=$(patsubst %.cc,%,$(wildcard *.cc))
EXE=$(patsubst %.cc,%,$(wildcard *.cc))

all: $(EXE)

clean:
	-rm -f *.o $(EXE)
