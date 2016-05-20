
.PHONY : clean distclean all

all : trapezoidal.so fem1d

trapezoidal.so : trapezoidal.cpp trapezoidal.h abstract_quadrature.h
	$(CXX) $(CPPFLAGS) -fPIC -shared $(CXXFLAGS) trapezoidal.cpp -o trapezoidal.so

abstract_quadrature.o : abstract_quadrature.cpp abstract_quadrature.h
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $<

fem1d : fem1d.o mesh.o abstract_quadrature.o
	$(CXX) $(LDFLAGS) fem1d.o mesh.o -o $@ $(LIBS)

fem1d.o mesh.o : %.o : %.cpp %.h $(HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $<

clean :
	$(RM) abstract_quadrature.o fem1d.o mesh.o trapezoidal.o 

distclean : clean
	$(RM) trapezoidal.so fem1d
