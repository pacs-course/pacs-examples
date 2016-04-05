OBJS =  matrix.o
TEST_OBJS = test_matrix_mult.o
HEADERS = matrix.h

libmatrix.a : matrix.o
	$(AR) crv libmatrix.a matrix.o

test_matrix_mult : libmatrix.a test_matrix_mult.o 
	$(CXX) -L. $(LDFLAGS) -o $@  test_matrix_mult.o -lmatrix $(LIBS)

test_gauss_seidel : libmatrix.a test_gauss_seidel.o 
	$(CXX) -L. $(LDFLAGS) -o $@  test_gauss_seidel.o -lmatrix $(LIBS)

test_solve : libmatrix.a test_solve.o 
	$(CXX) -L. $(LDFLAGS) -o $@  test_solve.o -lmatrix $(LIBS)

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
	$(RM) test_matrix_mult speedup.pdf libmatrix.a
