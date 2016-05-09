
.PHONY : clean distclean all

all : test_adaptive_quadrature adaptive_quadrature.so midpoint.so trapezoidal.so fem1d

adaptive_quadrature.so : adaptive_quadrature.cpp adaptive_quadrature.h
	$(CXX) $(CPPFLAGS) -fPIC -shared $(CXXFLAGS) adaptive_quadrature.cpp -o adaptive_quadrature.so

midpoint.so : midpoint.cpp midpoint.h
	$(CXX) $(CPPFLAGS) -fPIC -shared $(CXXFLAGS) midpoint.cpp -o midpoint.so

trapezoidal.so : trapezoidal.cpp trapezoidal.h
	$(CXX) $(CPPFLAGS) -fPIC -shared $(CXXFLAGS) trapezoidal.cpp -o trapezoidal.so

test_adaptive_quadrature.o : test_adaptive_quadrature.cpp 
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c test_adaptive_quadrature.cpp

test_adaptive_quadrature : test_adaptive_quadrature.o
	$(CXX) $(LDFLAGS) -o test_adaptive_quadrature test_adaptive_quadrature.o -ldl 

fem1d : fem1d.o mesh.o
	$(CXX) $(LDFLAGS) fem1d.o mesh.o -o $@ $(LIBS)

fem1d.o mesh.o : %.o : %.cpp %.h $(HEADERS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $<

clean :
	$(RM) adaptive_quadrature.o test_adaptive_quadrature.o fem1d.o mesh.o midpoint.o trapezoidal.o 

distclean : clean
	$(RM) test_adaptive_quadrature adaptive_quadrature.so midpoint.so trapezoidal.so fem1d
