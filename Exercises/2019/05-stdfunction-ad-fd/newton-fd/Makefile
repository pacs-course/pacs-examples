CXX       ?= g++
CXXFLAGS  ?= -std=c++11 
CPPFLAGS  ?= -I. -Wno-conversion-null -Wno-deprecated-declarations
CPPFLAGMP ?= -I$(mkMuparserInc)
EXECMP     = main_getpot_muparser 
LDFLAGS   ?= 
LDFLAGMP  ?= -L$(mkMuparserLib)
LIBS      ?= 
LIBMP     ?= -lmuparser


all : $(EXECMP) fd_demo

%.o : %.cpp newton.h
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $<

main_getpot_muparser.o : main_getpot_muparser.cpp newton.h muparser_fun.h
	$(CXX) -c $(CPPFLAGS) $(CPPFLAGMP) $(CXXFLAGS) $<

main_getpot_muparser : main_getpot_muparser.o 
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(LDFLAGMP) $< $(LIBS) $(LIBMP) -o $@

fd_demo.o : %.o : %.cpp newton.h muparser_fun.h
	$(CXX) -c $(CPPFLAGS) $(CPPFLAGMP) $(CXXFLAGS) $<

fd_demo : % : %.o 
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(LDFLAGMP) $< $(LIBS) $(LIBMP) -o $@

clean :
	$(RM) *.o

distclean : clean
	$(RM) $(EXECMP)


