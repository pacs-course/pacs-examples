CXX      ?= g++
CXXFLAGS ?= -std=c++17
CPPFLAGS ?= -O3 -Wall -pedantic -I. -I${mkEigenInc} -fPIC
LDFLAGS  ?= -L. -Wl,-rpath=$(PWD)
LDLIBS   ?= -lnewton
LINK.o := $(LINK.cc) # Use C++ linker.

EXEC=main
SRCS = $(wildcard *.cpp)
OBJS=Jacobian.o Newton.o
LIBNEWTON=libnewton.so

DEPEND = make.dep

.PHONY = all $(EXEC) $(LIBNEWTON) clean distclean $(DEPEND)

all: $(DEPEND) $(EXEC)

$(EXEC): $(EXEC).o $(LIBNEWTON)

$(LIBNEWTON): $(OBJS)
	$(CXX) $(LDFLAGS) -shared -Wl,-soname,$(LIBNEWTON) $^ -o $(LIBNEWTON)

*.o : %.o : %.cpp

clean:
	$(RM) $(DEPEND)
	$(RM) *.o

distclean: clean
	$(RM) $(EXEC) $(LIBNEWTON)
	$(RM) *.csv *.out *.bak *~

$(DEPEND): $(SRCS)
	@$(RM) $(DEPEND)
	@for file in $(SRCS); \
	do \
	  $(CXX) $(CPPFLAGS) $(CXXFLAGS) -MM $${file} >> $(DEPEND); \
	done

-include $(DEPEND)
