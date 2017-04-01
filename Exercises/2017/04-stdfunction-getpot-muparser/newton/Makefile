CXX       ?= g++
CXXFLAGS  ?= -std=c++11 
CPPFLAGS  ?= -I. -Wno-conversion-null -Wno-deprecated-declarations
CPPFLAGMP ?= -I$(mkMuparserInc)
EXEC       = main_fcn_ptr main_lambda main_getpot_lambda 
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

$(EXEC) : % : %.o 
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $< $(LIBS)

$(EXECMP) : % : %.o 
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(LDFLAGMP) $< $(LIBS) $(LIBMP)

clean :
	$(RM) *.o

distclean : clean
	$(RM) $(EXEC)


