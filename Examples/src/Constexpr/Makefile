#Assembly code
CXX=g++
CXXFLAGS=-O3 -std=c++11
OUTPUT=mainConstexprFunction mainNormalFunction mainConstexprFunction2
all : $(OUTPUT)
mainConstexprFunction: mainConstexprFunction.cpp
mainConstexprFunction2: mainConstexprFunction2.cpp
mainNormalFunction: mainNormalFunction.cpp twoFunctions.o
examine:
clean:
	$(RM) $(OUTPUT) *.o 
