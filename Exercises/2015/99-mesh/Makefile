ROOT=..
#
# Here possible additional libraries
#
MAIN_LIB_DIR=$(ROOT)/lib
#
# Here possible additioal header files 
#
MAIN_INC_DIR=$(ROOT)/include
#
# The compiler used
#
CXX = g++ 
#
# Since the linker is by defualt set to CC
# we make this workaround (we can do better)
#
CC  = $(CXX)
#
# I want always full warnings I 
#
WARNFLAGS= -Wall
#
# Additional optimization
#
OPTFLAGS += -g
# -g       generate code suitable for the debugger
# -p       generate code suitable for the profiler
# -O       attemp code optimizations

#
# where to look for include files
#
INCLS  = -I. -I./include -I$(MAIN_INC_DIR) 
#
# Preprocessor and compilation flags
#
CXXFLAGS=$(OPTFLAGS) $(WARNFLAGS)
CPPFLAGS=$(OPTFLAGS) $(INCLS)
#
# Additional libraries
#LIBS = -lm                   # for math library
LIBS = 
#
#Linker flags
#
LDFLAGS += $(LIBS) $(OPTFLAGS)
#
# get all files *.cpp and identify the main
#
SRCS=$(wildcard *.cpp)
EXE_SRC=$(filter main%.cpp,$(SRCS))
OTHER_SRCS=$(filter-out $(EXE_SRC),$(SRCS))
#
# get the corresponding object file
#
MAIN_OBJ=$(EXE_SRC:.cpp=.o)
OTHER_OBJS = $(OTHER_SRCS:.cpp=.o)
OBJS= $(MAIN_OBJ) $(OTHER_OBJS)
#
# name of the file containing include dependencies
#
DEPEND=make.dep
#
# 
# Call the executable with the sam name as the main file
#
EXEC=$(EXE_SRC:.cpp=)


#========================== TARGET DEFINITIONS
.phony= all  clean cleantemp veryclean

all: $(DEPEND) $(EXEC)

clean:
	-\rm -f $(EXEC) $(OBJS) $(DEPEND)

cleantemp:
	-\rm -f *~ *.bak

veryclean: clean cleantemp

#
# We assume that the main program depends on all
# the object files
#

$(EXEC): $(OBJS)


#
#generate dependencies (silently)
#
$(DEPEND): $(SRCS)
	@g++ $(INCLS) -MM $(SRCS) -MF $(DEPEND)

-include $(DEPEND)





