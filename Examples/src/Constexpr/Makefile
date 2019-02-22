#Assembly code
include ../../Makefile.inc
CXXFLAGS=-O3 -std=c++14
OUTPUT=mainConstexprFunction mainNormalFunction mainConstexprFunction2
all : $(OUTPUT)
mainConstexprFunction: mainConstexprFunction.cpp
mainConstexprFunction2: mainConstexprFunction2.cpp
mainNormalFunction: mainNormalFunction.cpp twoFunctions.o
examine:
clean:
	$(RM) $(OUTPUT) *.o 
