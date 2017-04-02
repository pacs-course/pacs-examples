IDIR =../include
CC=gcc
CXX=g++
CFLAGS=-I$(IDIR)
CXXFLAGS=-I$(IDIR)
LDFLAGS=

ODIR=../obj
LDIR =../lib
BINDIR =../bin

LIBS=

_DEPS = hellomake.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = hellomake.o hellofunc.o 
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/%.o: %.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

$(BINDIR)/hellomake: $(OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS) $(LIBS)

.PHONY: clean distclean

clean:
	$(RM) $(ODIR)/*.o *~ core $(INCDIR)/*~ 

distclean: clean
	$(RM) $(BINDIR)/hellomake 
