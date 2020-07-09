#g++ -g -Wall -std=c++11 -I. -I/usr/local/include/eigen3 -I../../Utilities -I../include tcg.cc -o tcg
export RELEASE=yes
include ../../../../Makefile.inc
-include Makefile.inc
EIGENDIR=$(EIGEN_DIR)
MMDIR=../../Utilities
THISINCDIR=../include
UMFPACK_INCDIR=/usr/include/suitesparse
LDLIBS= -lumfpack -lamd -L$(PACS_LIB_DIR) -lpacs
LDFLAGS=$(OPTFLAGS) -Wl,-rpath=$(PACS_LIB_DIR)
INCLUDES= -I. -I$(THISINCDIR) -I$(MMDIR) -I$(EIGEN_DIR) -I$(UMFPACK_INCDIR) 
CPPFLAGS+=$(INCLUDES)
vpath %.h $(THISINCDIR) $(MMDIR) $(EIGEN_DIR) $(UMFPACK_INCDIR)
vpath %.hpp . $(THISINCDIR) $(MMDIR) $(EIGEN_DIR) $(UMFPACK_INCDIR)
#
# By default I want optimization
#
EXE=main
SRCS=$(wildcard *.cpp)
OBJS=$(SRCS:.cpp=.o)

all: $(DEPEND) $(EXE)

$(EXE): $(OBJS)

clean:
	-rm -f *.o $(EXE)

$(DEPEND): $(SRCS)
	-\rm $(DEPEND)
	for f in $(SRCS); do \
	$(CXX) $(STDFLAGS) $(CPPFLAGS) -MM $$f >> $(DEPEND); \
	done

-include $(DEPEND)

