fem1d : fem1d.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $<
	$(CXX) $(LDFLAGS) $@.o -o $@ $(LIBS)

.PHONY: clean distclean

clean :
	$(RM) fem1d.o

distclean : clean
	$(RM) fem1d
