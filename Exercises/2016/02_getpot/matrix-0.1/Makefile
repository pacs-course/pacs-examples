OBJS =  matrix.o test_matrix_mult.o
HEADERS = matrix.h

test_matrix_mult : test_matrix_mult.cpp $(HEADERS) $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $(OBJS) $(LIBS)

libmatrix.a : matrix.o
	$(AR) crv libmatrix.a matrix.o

speedup.pdf : run_test.m
	octave $<

$(OBJS) : %.o : %.cpp %.h 
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $<

.PHONY: clean distclean

clean :
	$(RM) $(OBJS)

distclean : clean
	$(RM) test_matrix_mult speedup.pdf libmatrix.a
