CXX        = g++
CXXFLAGS  ?= -std=c++11 
CPPFLAGS  ?= -I. 
EXEC       = main_getpot_ad ad_demo
LDFLAGS   ?= 
LIBS      ?= 


all : $(EXEC) 

ad.o : ad.cpp ad.h
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $<

main_getpot_ad.o : main_getpot_ad.cpp newton.h ad.h
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $<

main_getpot_ad : main_getpot_ad.o ad.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ $(LIBS) -o $@

ad_demo.o : ad_demo.cpp ad.h
	$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $<

ad_demo : ad_demo.o ad.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ $(LIBS) -o $@


clean :
	$(RM) $(OBJS)

distclean : clean
	$(RM) $(EXEC)


