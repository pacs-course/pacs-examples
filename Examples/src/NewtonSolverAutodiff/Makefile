MAKEFILEH_DIR=../../
include $(MAKEFILEH_DIR)/Makefile.inc

SRCS = Newton_ad.cpp main_NewtonAutodiff.cpp
OBJS = $(SRCS:.cpp=.o)
EXEC = main_NewtonAutodiff

.PHONY: all clean
all: $(EXEC)

$(EXEC): $(OBJS)

clean:
	$(RM) -f $(EXEC) $(OBJS) make.dep

make.dep: $(SRCS)
	$(RM) -f $@
	for f in $(SRCS); do $(CXX) $(STDFLAGS) $(CPPFLAGS) -MM $$f >> $@; done

-include make.dep
