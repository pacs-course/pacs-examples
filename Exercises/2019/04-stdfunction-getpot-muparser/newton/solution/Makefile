CXX       ?= g++
CXXFLAGS  ?= -std=c++11 
CPPFLAGS  ?= -I. -Wno-conversion-null -Wno-deprecated-declarations
CPPFLAGMP ?= -I/usr/local/include
EXEC       = main_fcn_ptr main_lambda main_getpot_lambda 
EXECMP     = main_getpot_muparser
LDFLAGS   ?= 
LDFLAGMP  ?= -L/usr/local/lib
LIBS      ?= 
LIBMP     ?= -lmuparser

.PHONY : all clean distclean

all : $(EXEC) $(EXECMP)

%.o : %.cpp newton.h
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $<

$(EXECMP).o : %.o : %.cpp newton.h muparser_fun.h
	$(CXX) -c $(CPPFLAGS) $(CPPFLAGMP) $(CXXFLAGS) $<

$(EXEC) : % : %.o 
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $< $(LIBS) -o $@

$(EXECMP) : % : %.o 
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(LDFLAGMP) $< $(LIBS) $(LIBMP) -o $@

clean :
	$(RM) *.o

distclean : clean
	$(RM) $(EXEC) $(EXECMP)


