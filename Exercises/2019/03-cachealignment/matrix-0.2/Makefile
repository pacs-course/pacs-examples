OBJS =  matrix.o
CPPFLAGS ?= -I. -I/opt/local/include/eigen3
TEST_OBJS = test_matrix_mult.o test_solve.o
TESTS = test_matrix_mult test_solve
HEADERS = matrix.h
SHARED_EXT = so

.PHONY : tests

tests : $(TESTS)

libmatrix.a : $(OBJS)
	$(AR) crv libmatrix.a $(OBJS)

libmatrix.$(SHARED_EXT) : $(OBJS)
	$(CXX) -shared -L. $(LDFLAGS) -o $@ $(OBJS) $(LIBS)

test_matrix_mult : test_matrix_mult.o libmatrix.a 
	$(CXX) -L. $(LDFLAGS) -o $@  $< -lmatrix $(LIBS)

test_solve : test_solve.o libmatrix.a 
	$(CXX) -L. $(LDFLAGS) -o $@ $< -lmatrix $(LIBS)

$(OBJS) : %.o : %.cpp %.h 
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $<

$(TEST_OBJS) : %.o : %.cpp %.h 
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $<

.PHONY: clean distclean

clean :
	$(RM) $(OBJS) $(TEST_OBJS)

distclean : clean
	$(RM) $(TESTS) \
	libmatrix.a libmatrix.$(SHARED_EXT)
