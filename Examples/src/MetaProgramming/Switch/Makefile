############################################################
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
#
# Qui si trovano sono le librerie usate nel corso
#
MAIN_LIB_DIR=$(ROOT)/lib
#
# Qui gli include file usati nel corso
# (attenzione normalmente essi sono in una sottodirectory)
#
MAIN_INC_DIR=$(ROOT)/include
#
# Il compilatore c++ usato
#
CXX = g++ 
#
# Se CXX=icc voglio usare il compilatore intel
#
ifeq ($(findstring icc, $(CXX)),icc)
inteldir=$(dir $(CXX))
CC=CXX
LDFLAGS=-L/usr/local/opt/intel/cc/9.0/lib
endif
# Make usa la variabile CC anche per indicare il loader.
# Per semplicita la metto pari a CXX 
# 
CC =$(CXX)

OPTFLAGS = -g
# -c       compile only, generate corresponding .o files
# -g       generate code suitable for the debugger
# -p       generate code suitable for the profiler
# -O       attemp code optimizations

# where to look for include files
INCLS  = -I. -I$(MAIN_INC_DIR) 

CXXFLAGS=$(OPTFLAGS) $(INCLS)
#
# Nel caso usassimo il C 
CPPFLAGS=$(CXXFLAGS)

#
# Nome del file che viene incluso e contiene le dipendenze
# dagli headers autogenerate 
#
DEPEND=make.dep
# Additional libraries
#LIBS = -lm                   # for math library
LIBS =-lm 


#header files
LDFLAGS += $(LIBS)

# get all files *.cpp
SRCS=$(wildcard *.cpp)
# get the corresponding object file
OBJS = $(SRCS:.cpp=.o)
# get all headers in the working directory
HEADERS=$(wildcard *.hpp)

# In questo corso, per semplicita`, gli eseguibili vengono sempre chiamati 'main*.cpp'
# chiaramente non e` obbligatorio (ne consigliabile in generale)!
# Si assume anche che ce ne sia solo uno per directory
#
exe_sources=$(filter main%.cpp,$(SRCS))
EXEC=$(exe_sources:.cpp=)

#========================== ORA LA DEFINIZIONE DEGLI OBIETTIVI
.phony= all  clean

all: $(DEPEND) $(EXEC)

clean:
	-\rm -f $(EXEC) $(OBJS)
# Assumiamo che i MAIN dipendono da tutti i *.o e da tutti gli header nella
# directory corrente
# Questo e` vero nei casi piu` semplici. Altrimenti si modifichi di volta in volta 
# a seconda del caso

$(EXEC): $(OBJS)

$(OBJS): $(SRCS)

$(DEPEND): $(SRCS)
	g++ $(CXXFLAGS) -MM $(SRCS) -MF $(DEPEND)

-include $(DEPEND)




