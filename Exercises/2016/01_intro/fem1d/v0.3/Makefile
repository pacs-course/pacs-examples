HEADERS = fem1d.h mesh.h gauss_seidel.h config.h

fem1d : fem1d.cpp $(HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $<
	$(CXX) $(LDFLAGS) $@.o -o $@ $(LIBS)

.PHONY: clean distclean

clean :
	$(RM) fem1d.o

distclean : clean
	$(RM) fem1d
