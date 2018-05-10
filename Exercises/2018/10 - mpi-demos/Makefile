CPPFLAGS = -I.
CXXFLAGS = -std=c++11
CXX = mpicxx

all : sparse_matrix_assembly

sparse_matrix.o : sparse_matrix.cpp sparse_matrix.h
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c sparse_matrix.cpp

sparse_matrix_assembly.o : sparse_matrix_assembly.cpp sparse_matrix_assembly.h sparse_matrix.h
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c sparse_matrix_assembly.cpp

sparse_matrix_assembly_helper.o : sparse_matrix_assembly_helper.cpp sparse_matrix_assembly.h sparse_matrix.h
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c sparse_matrix_assembly_helper.cpp

sparse_matrix_assembly : sparse_matrix_assembly.o sparse_matrix_assembly_helper.o sparse_matrix.o
	$(CXX) sparse_matrix_assembly.o sparse_matrix_assembly_helper.o sparse_matrix.o -o sparse_matrix_assembly

clean :
	$(RM) sparse_matrix_assembly.o sparse_matrix_assembly_helper.o sparse_matrix.o

distclean : clean
	$(RM) sparse_matrix_assembly
