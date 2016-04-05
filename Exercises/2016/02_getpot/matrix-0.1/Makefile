OBJS =  matrix.o
TEST_OBJS = test_matrix_mult.o test_gauss_seidel.o test_solve.o
HEADERS = matrix.h
SHARED_EXT = so

libmatrix.a : $(OBJS)
	$(AR) crv libmatrix.a $(OBJS)

libmatrix.$(SHARED_EXT) : $(OBJS)
	$(CXX) -shared -L. $(LDFLAGS) -o $@ $(OBJS) $(LIBS)

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
	$(RM) test_matrix_mult test_gauss_seidel test_solve \
	speedup.pdf libmatrix.a libmatrix.$(SHARED_EXT)
