heat_exchange.pdf:
	(cd Figure; make)
	pdflatex heat_exchange.tex
mkBoostInc?=/usr/include
mkBoostLib?=/usr/lib/x86_64-linux-gnu

CPPFLAGS+=-I$(mkBoostInc)
LDLIBS+=-L$(mkBoostLib) -lboost_iostreams -lboost_system -lboost_filesystem
