CXX = mpicxx
OBJS = fem1d.o 
HEADERS = 
CPPFLAGS ?= -I. -I/opt/local/include/eigen3 -I/opt/muparser/2.2.5/include/
LDFLAGS ?= 
LIBS = 

fem1d : $(OBJS)
	$(CXX) $(LDFLAGS) $(OBJS) -o $@ $(LIBS)

$(OBJS) : %.o : %.cpp %.h $(HEADERS)
	echo $(CXX)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $<

.PHONY: clean distclean

clean :
	$(RM) $(OBJS)

distclean : clean
	$(RM) fem1d
