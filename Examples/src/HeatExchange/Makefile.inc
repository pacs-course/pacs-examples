heat_exchange.pdf:
	(cd Figure; make)
	pdflatex heat_exchange.tex
LDLIBS+=-lboost_iostreams -lboost_system -lboost_filesystem