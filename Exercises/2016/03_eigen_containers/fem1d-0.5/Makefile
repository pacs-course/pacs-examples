OBJS = fem1d.o mesh.o
HEADERS = 
CPPFLAGS ?= -I. -I/opt/local/include/eigen3
LDFLAGS ?= 
LIBS = 

fem1d : $(OBJS)
	$(CXX) $(LDFLAGS) $(OBJS) -o $@ $(LIBS)

$(OBJS) : %.o : %.cpp %.h $(HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $<

.PHONY: clean distclean

clean :
	$(RM) $(OBJS)

distclean : clean
	$(RM) fem1d
