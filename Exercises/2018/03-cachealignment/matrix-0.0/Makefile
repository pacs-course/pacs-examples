OBJS =  matrix.o
HEADERS = matrix.h
SHARED_EXT = so

.PHONY : tests

tests : $(TESTS)

libmatrix.a : $(OBJS)
	$(AR) crv libmatrix.a $(OBJS)

libmatrix.$(SHARED_EXT) : $(OBJS)
	$(CXX) -shared -L. $(LDFLAGS) -o $@ $(OBJS) $(LIBS)

$(OBJS) : %.o : %.cpp %.h 
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $<

$(TEST_OBJS) : %.o : %.cpp %.h 
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $<

.PHONY: clean distclean

clean :
	$(RM) $(OBJS)

distclean : clean
	$(RM) $(TESTS) \
	libmatrix.a libmatrix.$(SHARED_EXT)
