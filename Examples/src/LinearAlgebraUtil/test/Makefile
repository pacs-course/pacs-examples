include ../../../Makefile.inc
# I need -I.. at the beginning
OCPPFLAGS:=$(CPPFLAGS)
CPPFLAGS=-I.. $(OCPPFLAGS)
# for multithreading
#LDLIBS+=-L$(TBB_LIBDIR) -ltbb#
# for openmp
CXXFLAGS+=-fopenmp
LDFLAGS+=-fopenmp
# You may have an include file also in the current directory
#
-include Makefile.inc

#
# get all files *.cpp
SRCS=$(wildcard test*.cpp)
# get the corresponding object file
OBJS = $(SRCS:.cpp=.o)
exe_sources=$(SRCS)
EXEC=$(exe_sources:.cpp=)

all: $(DEPEND) $(EXEC)
exec: $(DEPEND) $(EXEC)

$(EXEC):%: %.o

$(DEPEND): $(SRCS)
	-\rm $(DEPEND)
	for f in $(SRCS); do \
	$(CXX) $(STDFLAGS) $(CPPFLAGS) -MM $$f >> $(DEPEND); \
	done

clean:
	$(RM) $(OBJS) $(EXEC)
distclean: clean
	$(RM) $(DEPEND)

-include $(DEPEND)
