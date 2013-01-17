# g++ -fPIC -I. -IQuadrature -c myRules.cpp
#g++ -shared -Wl,-soname,libquadrules.so -o libquadrules.so myRules.o
# g++ -rdynamic -c  -I. -IQuadrature main_integration.cpp
#g++ -rdynamic -o main_integration main_integration.o -L. -lfactory -L./Quadrature -lnumint -lquadrules -ldl
#########################################################
#
# Esempio di Makefile usato per il corso 
# Programmazione avanzata per il calcolo scientifico
# Da modificare a seconda dei casi 
#
############################################################
#
# Nella variabile d'ambiente PACS_ROOT e` contenuta la directory di base 
# dove sono messe le utilities (librerie e include files) usati nel corso e
# non presenti nel folder dove si sta lavorando. 
# Se non e` settata si considera un valore di default (/usr/local)
#
ifeq ($(PACS_ROOT),)
ROOT=/usr/local
else
ROOT=$(PACS_ROOT)
endif
# Se viene specificato LOAD=NONE  la libreria sara` caricata staticamemente
# per farlo dinamicamente usare make LOAD=DYNAMIC 
#
LOAD=DYNAMIC
#
# Il compilatore c++ usato
#
CXX = g++ 
# Make usa la variabile CC anche per indicare il loader.
# Per semplicita la metto pari a CXX 
# 
CC =$(CXX)

OPTFLAGS = -O2 -msse2 -mfpmath=sse
# -c       compile only, generate corresponding .o files
# -g       generate code suitable for the debugger
# -p       generate code suitable for the profiler
# -O       attemp code optimizations

# where to look for include files
INCLS  = -I. -IQuadrature 

CXXFLAGS=-fPIC
CXXFLAGS += $(OPTFLAGS) 
#
# Flags per il preprocessore
# 
CPPFLAGS= $(INCLS)

#
# Nome del file che viene incluso e contiene le dipendenze
# dagli headers autogenerate 
#
DEPEND=make.dep
SRCS=
# get the corresponding object file
OBJS = $(SRCS:.cpp=.o)
# get all headers in the working directory
HEADERS=$(SRCS:.cpp=.hpp)

# Additional libraries
#LIBS = -lm                   # for math library
LDLIBS = 

# In questo corso, per semplicita`, gli eseguibili vengono sempre chiamati
# 'main*.cpp' chiaramente non e` obbligatorio (ne consigliabile in
# generale)!  Si assume anche che ce ne sia solo uno per directory
#
ALLSRCS+=$(wildcard *.cpp)
EXESRCS=$(filter main%.cpp,$(ALLSRCS))
OTHERSRC=$(filter-out main%.cpp,$(ALLSRCS))
TMP:=$(OTHERSRC)
OTHERSRC=$(filter-out myRules%.cpp,$(TMP))
OTHEROBJS= $(OTHERSRC:.cpp=.o)
HEADERS=$(wildcard *.hpp)
SRCS=myRules.cpp
OBJS=myRules.o
EXEC=$(EXESRCS:.cpp=)
EXEOBJS=$(EXESRCS:.cpp=.o)
#
UDFLIBDYN=libudf.so
LIBDYN=libquadrules.so
#
LIBOBJS=$(OTHEROBJS)
#
# Aggiungere la nostra libreria
LDLIBS += -lm -LQuadrature -lnumint -L. 
ifeq ($(LOAD),NONE)
LDLIBS += -lquadrules
else
CPPFLAGS+= -DDYNAMIC
LDLIBS += -ldl
endif 

#========================== ORA LA DEFINIZIONE DEGLI OBIETTIVI
.phony= all  clean doc

all:
	(cd Quadrature; make)
	$(MAKE) -f Makefile this

this: $(DEPEND) $(LIBDYN) $(EXEC)


doc:
	doxygen

clean:
	(cd Quadrature; make clean)
	\rm -f $(EXEC) $(LIBDYN) *.o
	\rm -r -f doc *.o
# Assumiamo che i MAIN dipendono da tutti i *.o e da tutti gli header nella
# directory corrente
# Questo e` vero nei casi piu` semplici. Altrimenti si modifichi di volta in volta 
# a seconda del caso

$(EXEC): $(EXEOBJS)
#	g++ -o $(EXEC) $(EXEOBJS)  $(OPTFLAGS) $(LDLIBS)
#	@echo "****************************************************"
#	@echo "ricordarsi di settare export LD_LIBRARY_PATH=.:./Quadrature"
#	@echo "****************************************************"
	g++ -o $(EXEC) -Wl,-rpath,.:./Quadrature $(EXEOBJS)  $(OPTFLAGS) $(LDLIBS)

$(EXEOBJS): $(EXESRCS)
	g++ -c  $(CPPFLAGS) $(EXESRCS) 

$(LIBDYN) : $(OBJS)
	g++ -Wall -shared -Wl,-soname,$(LIBDYN) -o $(LIBDYN)  $(OBJS)

$(OBJS) :  $(SRCS)
	g++  $(OPTFLAGS) $(CPPFLAGS) -fPIC  -c  $(SRCS)

$(DEPEND): $(ALLSRCS) $(HEADERS)
	g++ $(CPPFLAGS) -MM $(SRCS)  > $(DEPEND)

-include $(DEPEND)




