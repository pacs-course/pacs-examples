############################################################
# Some examples modified from
# C++ and Object Oriented Numeric Computing
# for Scientists and Engineers.
#               Daoqi Yang  August 17, 2001 
############################################################

BASE = $(PWD)

#CXX  = g++
CXX  = /opt/clang/bin/clang++

CC   = $(CXX) 

OPTFLAGS = -O3
# -c       compile only, generate corresponding .o files
# -g       generate code suitable for the debugger
# -p       generate code suitable for the profiler
# -O       attemp code optimizations

# where to look for include files
INCLS  = 

CXXFLAGS=$(OPTFLAGS) -Wall $(INCLS)


# directory to find libraries or libraries to be used
LIBS =-lm 

#LIBS = -lm                   # for math library
#      -lrpcsvc               # for function "rstat" 
#LIBS = -lm $(BASE)/lib/linpack.a
# To make a library:
#    ar ruv mylib.a file1.o file2.o   and:  ranlib mylib.a
# To see a libaray: ar t mylib.a

#header files
LDFLAGS=$(LIBS)

.PHONY= clean all
HDRS = 

SRCS=$(wildcard *.cpp)

OBJS = $(SRCS:.cpp=.o)

EXEC=$(OBJS:.o=)

all: $(EXEC)

clean:
	-\rm -f $(OBJS) $(EXEC)



