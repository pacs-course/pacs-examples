CXX      ?= g++
CXXFLAGS ?= -std=c++17
CPPFLAGS ?= -O3 -Wall -pedantic -I. -fPIC
LDFLAGS  ?= -L. -Wl,-rpath=$(PWD)
LINK.o := $(LINK.cc) # Use C++ linker.

EXEC = main_quadrature
SRCS = $(wildcard *.cpp)
LIBS = adaptive_quadrature.so midpoint.so trapezoidal.so

DEPEND = make.dep

.PHONY = all $(EXEC) $(LIBS) clean distclean $(DEPEND)

all: $(DEPEND) $(EXEC) $(LIBS)

$(EXEC): $(EXEC).o
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) -ldl $< -o $@

%.so: %.o
	$(CXX) $(LDFLAGS) -shared -Wl,-soname,$@ $< -o $@

clean:
	$(RM) $(DEPEND)
	$(RM) *.o *.so

distclean: clean
	$(RM) $(EXEC) *.so
	$(RM) *.csv *.out *.bak *~

$(DEPEND): $(SRCS)
	@$(RM) $(DEPEND)
	@for file in $(SRCS); \
	do \
	  $(CXX) $(CPPFLAGS) $(CXXFLAGS) -MM $${file} >> $(DEPEND); \
	done

-include $(DEPEND)
