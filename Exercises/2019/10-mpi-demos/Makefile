CPPFLAGS = -I.
CXXFLAGS = -std=c++11
CXX = mpicxx

all : sparse_matrix_assembly finite_elements_assembly

sparse_matrix_simplified.o : sparse_matrix_simplified.cpp sparse_matrix_simplified.h
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c sparse_matrix_simplified.cpp

sparse_distributed.o : sparse_distributed.cpp sparse_distributed.h sparse_matrix_simplified.h
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c sparse_distributed.cpp

finite_elements_assembly.o : finite_elements_assembly.cpp sparse_distributed.h sparse_matrix_simplified.h
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c finite_elements_assembly.cpp

sparse_matrix_assembly_helper.o : sparse_matrix_assembly_helper.cpp sparse_matrix_assembly.h sparse_matrix_simplified.h
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c sparse_matrix_assembly_helper.cpp

sparse_matrix_assembly : sparse_matrix_assembly.o sparse_matrix_assembly_helper.o sparse_matrix_simplified.o
	$(CXX) sparse_matrix_assembly.o sparse_matrix_assembly_helper.o sparse_matrix_simplified.o -o sparse_matrix_assembly

finite_elements_assembly : finite_elements_assembly.o  sparse_matrix_simplified.o sparse_distributed.o
	$(CXX) finite_elements_assembly.o sparse_distributed.o sparse_matrix_simplified.o -o finite_elements_assembly

clean :
	$(RM) sparse_matrix_assembly.o sparse_matrix_assembly_helper.o sparse_matrix_simplified.o finite_elements_assembly.o

distclean : clean
	$(RM) sparse_matrix_assembly finite_elements_assembly
