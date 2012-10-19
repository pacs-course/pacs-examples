############################################################
# Makefile for 
# C++ and Object Oriented Numeric Computing
# for Scientists and Engineers.
#               Daoqi Yang  August 17, 2001 
############################################################

BASE = $(PWD)

CC   = g++ 

OPTFLAGS = -g
# -c       compile only, generate corresponding .o files
# -g       generate code suitable for the debugger
# -p       generate code suitable for the profiler
# -O       attemp code optimizations

# where to look for include files
INCLS  = 

CXXFLAGS=$(OPTFLAGS) $(INCLS)


# directory to find libraries or libraries to be used
LIBS = 

#LIBS =                    # for math library
#      -lrpcsvc               # for function "rstat" 
#LIBS =  $(BASE)/lib/linpack.a
# To make a library:
#    ar ruv mylib.a file1.o file2.o   and:  ranlib mylib.a
# To see a libaray: ar t mylib.a

#header files
LDFLAGS=$(LIBS)

.PHONY= clean all
HDRS = 

SRCS=$(wildcard *.cc)

OBJS = $(SRCS:.cc=.o)

EXEC=$(OBJS:.o=)

all: $(EXEC)

clean:
	-\rm $(OBJS) $(EXEC)



