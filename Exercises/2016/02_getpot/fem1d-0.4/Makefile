OBJS = fem1d.o mesh.o
HEADERS = 
CPPFLAGS ?= -I../matrix-0.1/ -I.
LDFLAGS ?= -L../matrix-0.1/
LIBS = 

fem1d : $(OBJS)
	$(CXX) $(LDFLAGS) $(OBJS) -o $@ -lmatrix $(LIBS)

$(OBJS) : %.o : %.cpp %.h $(HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $<

.PHONY: clean distclean

clean :
	$(RM) $(OBJS)

distclean : clean
	$(RM) fem1d
