#
# GENERAL C++ MAKEFILE
#
ROOT=..

# Change EXEC to the name of the executable which you wish to
# create. Make OFILES be the name of all the .C files in your program,
#
LIBSUFFIX=a

DEPEND=make.dep

ifdef DYNAMIC
LIBSUFFIX=so
CXXFLAGS=-fPIC 
else
LIBSUFFIX=a
endif

LIBDIR=$(ROOT)/lib

export DYNAMIC

SRCS=$(wildcard *.cpp)
OBJS=$(SRCS:.cpp=.o)
EXEC=$(SRCS:.cpp=)

LIBRARY=$(LIBDIR)/libDesignCPP.$(LIBSUFFIX)
#Compiler specifications
CXX = g++

CC = g++
DEBUG  =  -Wall -g
LDFLAGS = -L$(LIBDIR) -lDesignCPP -Wl,-rpath=$(LIBDIR)

CPPFLAGS   = -Wall -I$(ROOT)/include
CXXFLAGS  += $(DEBUG) 


# These are different make options.  You can invoke them by typing 
# the label name.  For example to time your make, type 'make time' -- 
# 'make' being the name of this command and 'time' being the label used 
# below to define the result.  
#
.PHONY= all objs dynamic static alldynamic allstatic


all: $(EXEC)

clean:
	$(RM) $(EXEC)

$(DEPEND): $(SRCS)
	g++ $(CPPFLAGS) -MM $(SRCS) -MF $(DEPEND)

-include $(DEPEND)






