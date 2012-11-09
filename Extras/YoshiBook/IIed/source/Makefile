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

LIBRARY=$(LIBDIR)/libDesignCPP.$(LIBSUFFIX)
#Compiler specifications
CXX = g++

CC = g++
DEBUG  =  -Wall -g
LDFLAGS = -L$(LIBDIR) -lDesignCPP

CPPFLAGS   = -Wall -I$(ROOT)/include
CXXFLAGS  += $(DEBUG) 

LDFLAGS += -lm

# These are different make options.  You can invoke them by typing 
# the label name.  For example to time your make, type 'make time' -- 
# 'make' being the name of this command and 'time' being the label used 
# below to define the result.  
#
.PHONY= all objs dynamic static alldynamic allstatic


default: dynamic static


static:
	-rm -f *.o
	$(MAKE) allstatic

dynamic:
	-rm -f *.o
	$(MAKE) DYNAMIC=1 alldynamic

allstatic: $(LIBRARY)($(OBJS))


alldynamic: objs dynlib

objs: $(OBJS)

dynlib:
	$(CXX) -shared -Wl,-soname=libDesignCPP.so $(OBJS) -o $(LIBRARY)



clean:
	$(RM) *.o ../lib/*.a ../lib/*.so 

$(DEPEND): $(SRCS)
	g++ $(CPPFLAGS) -MM $(SRCS) -MF $(DEPEND)

-include $(DEPEND)






