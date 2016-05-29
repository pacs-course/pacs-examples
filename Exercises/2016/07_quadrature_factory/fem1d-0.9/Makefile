
.PHONY : clean distclean all

all : trapezoidal.so abstract_quadrature.so fem1d 

abstract_quadrature.so : abstract_quadrature.cpp abstract_quadrature.h
	$(CXX) $(CPPFLAGS) -fPIC -shared $(CXXFLAGS) $< -o $@

trapezoidal.so : trapezoidal.cpp trapezoidal.h abstract_quadrature.h abstract_quadrature.so
	$(CXX) $(CPPFLAGS) -fPIC -shared $(CXXFLAGS) $< -o $@ abstract_quadrature.so

fem1d : fem1d.o mesh.o abstract_quadrature.so
	$(CXX) $(LDFLAGS) fem1d.o mesh.o -o $@ $(LIBS) abstract_quadrature.so

fem1d.o mesh.o : %.o : %.cpp %.h $(HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $<

clean :
	$(RM) abstract_quadrature.o fem1d.o mesh.o trapezoidal.o

distclean : clean
	$(RM) trapezoidal.so abstract_quadrature.so fem1d 
