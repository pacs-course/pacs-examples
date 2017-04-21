OBJS =  matrix.o
HEADERS = matrix.h
SHARED_EXT = so
LAPACK_FLAG = -DUSE_DGEMM
CPPFLAGS = $(LAPACK_FLAG)

libmatrix.a : $(OBJS)
	$(AR) crv libmatrix.a $(OBJS)

libmatrix.$(SHARED_EXT) : $(OBJS)
	$(CXX) -shared -L. $(LDFLAGS) -o $@ $(OBJS) $(LIBS)

test_lu : test_lu.o libmatrix.a 
	$(CXX) -L. $(LDFLAGS) -o $@ $< -lmatrix $(LIBS)

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
	speedup.pdf libmatrix.a libmatrix.$(SHARED_EXT) *~
