############################################################
# Makefile for 
# C++ and Object Oriented Numeric Computing
# for Scientists and Engineers.
#               Daoqi Yang  August 17, 2001 
############################################################

BASE = $(PWD)

CC   = g++ 

CFLAGS = -g
# -c       compile only, generate corresponding .o files
# -g       generate code suitable for the debugger
# -p       generate code suitable for the profiler
# -O       attemp code optimizations

# where to look for include files
INCLS  = 

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

ALLOBJS = $(SRCS:.cc=.o)

OBJS=$(subst ch4it.o,,$(ALLOBJS))

EXEC=$(OBJS:.o=)

LIB=libch4it.a(ch4it.o)

all: $(EXEC) $(LIB)

ch4it.o: ch4it.cc ch4it.h
	$(CC) $(CFLAGS) -I. -c ch4it.cc

ch4itmain: ch4itmain.cc ch4it.o
	$(CC) $(LDFLAGS) -o $% $^ 

clean:
	-\rm $(ALLOBJS) $(EXEC)



