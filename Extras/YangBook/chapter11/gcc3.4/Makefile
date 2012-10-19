############################################################
# Makefile for 
# C++ and Object Oriented Numeric Computing
# for Scientists and Engineers.
#               Daoqi Yang  July 17, 2001 
############################################################

BASE = $(PWD)

CC   = g++
#CC   = CC 

OPTFLAGS = -g
# -c       compile only, generate corresponding .o files
# -g       generate code suitable for the debugger
# -p       generate code suitable for the profiler
# -O       attemp code optimizations

# where to look for include files
INCLS  = -I.

CXXFLAGS=$(OPTFLAGS) $(INCLS)

# libraries to be used
LIBS =  -L. -l$(LIBSUF)
LDFLAGS=$(LIBS) $(OPTFLAGS)
#      -lrpcsvc               # for function "rstat" 
#LIBS =  $(BASE)/lib/linpack.a
# To make a library:
#    ar ruv mylib.a file1.o file2.o   and:  ranlib mylib.a
# To see a libaray: ar t mylib.a


#header files

.PHONY= clean all

SRCS=$(wildcard *.cc)

LIBSUF=ch11
LIBNAME=lib$(LIBSUF).a
LIBRARY=$(LIBNAME)(fdm.o matvec.o)

OBJS=fdmmain2.o fdmmain matvecmain

EXEC=fdmmain2 fdmmain matvecmain

all: $(LIBRARY) $(EXEC)

fdmmain2: $(LIBRARY) fdmmain2.o 
	$(CC)  fdmmain2.o $(LDFLAGS) -o $@

fdmmain: $(LIBRARY) fdmmain.o 
	$(CC) fdmmain.o $(LDFLAGS) -o $@

matvecmain: $(LIBRARY) matvecmain.o 
	$(CC) matvecmain.o $(LDFLAGS) -o $@

fdm.o: fdm.cc fdm.h matvec.h
	$(CC) $(CXXFLAGS) -c fdm.cc

matvec.o: matvec.cc matvec.h
	$(CC) $(CXXFLAGS) -c matvec.cc

clean:
	-\rm $(OBJS) *o $(EXEC) $(LIBNAME)

