#Assembly code
include ../../Makefile.inc
#CXXFLAGS=-O3 -std=c++17
OUTPUT=mainConstexprFunction mainNormalFunction mainConstexprFunction2  mainHornerConstexpr
all : $(OUTPUT)
mainConstexprFunction: mainConstexprFunction.o
mainConstexprFunction2: mainConstexprFunction2.o
mainNormalFunction: mainNormalFunction.o twoFunctions.o
mainHornerConstexpr: mainHornerConstexpr.o HornerConstexpr.hpp
clean:
	$(RM) $(OUTPUT) *.o 
distclean: clean

