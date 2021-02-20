# Change to suit your system 
mkTbbLib?=/usr/lib/x86_64-linux-gnu/
mkTbbInc?=/usr/include

CPPFLAGS+=-I. -I$(mkTbbInc)
LDLIBS+=-L${mkTbbLib} -ltbb
