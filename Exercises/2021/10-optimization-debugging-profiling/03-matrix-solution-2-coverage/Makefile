CXX      ?= g++
CXXFLAGS ?= -std=c++17
CPPFLAGS ?= -O0 -g --coverage -Wall -pedantic -I.
LDLIBS   ?= 
LINK.o := $(LINK.cc) # Use C++ linker.

DEPEND = make.dep

EXEC = test_matrix_mult
SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)

.PHONY = all $(EXEC) $(OBJS) coverage memcheck profile clean distclean $(DEPEND)

all: $(DEPEND) $(EXEC)

$(EXEC): $(OBJS)

$(OBJS): %.o: %.cpp

coverage: all
	lcov --directory . --zerocounters
	./$(EXEC)
	lcov --directory . --capture --no-external --output test_coverage.info
	genhtml test_coverage.info --output test_coverage

memcheck: all
	valgrind --tool=memcheck ./$(EXEC)

profile: all
	valgrind --tool=callgrind ./$(EXEC)

clean:
	$(RM) $(DEPEND)
	$(RM) -r *.o *.gcda *.gcno test_coverage* callgrind*

distclean: clean
	$(RM) $(EXEC)
	$(RM) *.csv *.out *.bak *~

$(DEPEND): $(SRCS)
	@$(RM) $(DEPEND)
	@for file in $(SRCS); \
	do \
	  $(CXX) $(CPPFLAGS) $(CXXFLAGS) -MM $${file} >> $(DEPEND); \
	done

-include $(DEPEND)
