doc = fin
dep = tridiag.hpp tridiag.cpp
cpp = g++

$(doc): $(doc).cpp $(dep)
	$(cpp) tridiag.cpp $(doc).cpp -llapack -O3 -o $(doc).exe

doc: $(doc).cpp $(dep) Doxyfile
	doxygen Doxyfile

clean: $(doc).exe 
	rm $(doc).exe
