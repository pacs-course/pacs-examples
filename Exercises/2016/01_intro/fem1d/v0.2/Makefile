OBJS = fem1d.o gauss_seidel.o mesh.o
HEADERS = config.h

fem1d : $(OBJS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $<
	$(CXX) $(LDFLAGS) $@.o $(OBJS) -o $@ $(LIBS)

$(OBJS) : %.o : %.cpp %.h $(HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $<

.PHONY: clean distclean

clean :
	$(RM) $(OBJS)

distclean : clean
	$(RM) fem1d
