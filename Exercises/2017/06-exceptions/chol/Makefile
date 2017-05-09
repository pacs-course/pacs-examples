OBJS =  matrix.o
HEADERS = matrix.h
SHARED_EXT = so
LAPACK_FLAG = -DUSE_DGEMM
CPPFLAGS = $(LAPACK_FLAG)

all : libmatrix.a test_chol

libmatrix.a : $(OBJS)
	$(AR) crv libmatrix.a $(OBJS)

libmatrix.$(SHARED_EXT) : $(OBJS)
	$(CXX) -shared -L. $(LDFLAGS) -o $@ $(OBJS) $(LIBS)

test_lu : test_lu.o libmatrix.a 
	$(CXX) -L. $(LDFLAGS) -o $@ $< -lmatrix $(LIBS)

test_chol : test_chol.o libmatrix.a
	$(CXX) -L. $(LDFLAGS) -o $@ $< -lmatrix $(LIBS)

test_solve : test_solve.o libmatrix.a 
	$(CXX) -L. $(LDFLAGS) -o $@ $< -lmatrix $(LIBS)

$(OBJS) : %.o : %.cpp %.h 
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $<

$(TEST_OBJS) : %.o : %.cpp %.h 
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $<

.PHONY: clean distclean

clean :
	$(RM) matrix.o test_lu.o test_chol.o

distclean : clean
	$(RM) $(TESTS) \
	libmatrix.a libmatrix.$(SHARED_EXT) test_chol test_lu *~
