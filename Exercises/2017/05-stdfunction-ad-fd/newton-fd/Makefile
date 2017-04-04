CXX       ?= g++
CXXFLAGS  ?= -std=c++11 
CPPFLAGS  ?= -I. -Wno-conversion-null -Wno-deprecated-declarations
CPPFLAGMP ?= -I$(mkMuparserInc)
EXECMP     = main_getpot_muparser
LDFLAGS   ?= 
LDFLAGMP  ?= -L$(mkMuparserLib)
LIBS      ?= 
LIBMP     ?= -lmuparser


all : $(EXEC) $(EXECMP)

%.o : %.cpp newton.h
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $<

$(EXECMP).o : %.o : %.cpp newton.h muparser_fun.h
	$(CXX) -c $(CPPFLAGS) $(CPPFLAGMP) $(CXXFLAGS) $<

$(EXECMP) : % : %.o 
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(LDFLAGMP) $< $(LIBS) $(LIBMP) -o $@

clean :
	$(RM) *.o

distclean : clean
	$(RM) $(EXECMP)


