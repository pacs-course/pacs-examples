#Assembly code
CXX=g++
CXXFLAGS=-O3 -std=c++11
OUTPUT=mainConstexprFunction mainNormalFunction
all : $(OUTPUT)
mainConstexprFunction: mainConstexprFunction.cpp
mainNormalFunction: mainNormalFunction.cpp twoFunctions.o
clean:
	$(RM) $(OUTPUT) *.o 
