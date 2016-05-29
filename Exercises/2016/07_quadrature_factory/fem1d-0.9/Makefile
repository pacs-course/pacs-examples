
.PHONY : clean distclean all

all : trapezoidal.so abstract_quadrature.so fem1d 

trapezoidal.so abstract_quadrature.so : %.so : %.cpp %.h abstract_quadrature.h
	$(CXX) $(CPPFLAGS) -fPIC -shared $(CXXFLAGS) $< -o $@

fem1d : fem1d.o mesh.o abstract_quadrature.so
	$(CXX) $(LDFLAGS) fem1d.o mesh.o abstract_quadrature.so -o $@ $(LIBS)

fem1d.o mesh.o main.o secondary.o : %.o : %.cpp %.h $(HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $<

clean :
	$(RM) abstract_quadrature.o fem1d.o mesh.o trapezoidal.o

distclean : clean
	$(RM) trapezoidal.so abstract_quadrature.so fem1d 
