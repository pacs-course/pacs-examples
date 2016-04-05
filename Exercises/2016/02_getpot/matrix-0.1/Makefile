OBJS =  matrix.o
TEST_OBJS = test_matrix_mult.o test_gauss_seidel.o test_solve.o
TESTS = test_matrix_mult test_gauss_seidel test_solve
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

test_gauss_seidel : test_gauss_seidel.o libmatrix.a
	$(CXX) -L. $(LDFLAGS) -o $@  $< -lmatrix $(LIBS)

test_solve : test_solve.o libmatrix.a 
	$(CXX) -L. $(LDFLAGS) -o $@ $< -lmatrix $(LIBS)

speedup.pdf : run_test.m
	octave $<

$(OBJS) : %.o : %.cpp %.h 
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $<

$(TEST_OBJS) : %.o : %.cpp %.h 
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $<

.PHONY: clean distclean

clean :
	$(RM) $(OBJS) $(TEST_OBJS)

distclean : clean
	$(RM) $(TESTS) \
	speedup.pdf libmatrix.a libmatrix.$(SHARED_EXT)
