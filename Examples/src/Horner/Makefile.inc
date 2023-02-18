# You have to set the threading building clok libraries if you fant parallel algorithm with g++

mkTbbLib?=/usr/lib/x86_64-linux-gnu/
mkTbbInc?=/usr/include

#CPPFLAGS+=-I. -I$(PACS_INC_DIR) -I$(mkTbbInc)
#LDLIBS+=-L${mkTbbLib} -ltbb
DEBUG=no
parallel:
	$(MAKE) all DEBUG=no CPPFLAGS+="-DPARALLELEXEC -I. -I$(PACS_INC_DIR) -I$(mkTbbInc)" LDLIBS+="-L${mkTbbLib} -ltbb"
