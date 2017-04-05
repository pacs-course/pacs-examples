rerun = "There were undefined references"
bibtex = "\citation"

doc = es1
dep = $(wildcard *.tex) $(src) $(fig)
bib = ../common/bibliography.bib

src = $(wildcard ./ex*/*.cpp) $(wildcard ./ex*/*.hpp)

fig = $(wildcard ./figures/eps/*.eps) $(wildcard ./figures/fig/*.fig)

DUST = *.aux *.log *.nav *.out *.dvi *.pdf *.ps *.snm *.toc *.bbl *.blg *.vrb *.txt *~

vpath %.eps ./figures/eps/
vpath %.fig ./figures/fig/
vpath %.hpp ./ex1:./ex2
vpath %.cpp ./ex1:./ex2

.PHONY = all pdf dvi ps tikz clean clean-all

all: pdf

pdf : $(doc).pdf

dvi : $(doc).dvi

ps : $(doc).ps

%.pdf : %.tex tikz $(dep)
	pdflatex $*
	if (egrep -q $(bibtex) $*.aux); then bibtex $* && pdflatex $*; fi
	pdflatex $* && !(egrep -q $(rerun) $*.log)

%.dvi : %.tex $(dep)
	latex $*
	if (egrep -q $(bibtex) $*.aux); then bibtex $* && latex $*; fi
	latex $* && !(egrep -q $(rerun) $*.log)

%.ps : %.dvi
	dvips $< -o $*.ps

tikz:
	cd figures/tikz/; make all

clean :
	for dustfile in $(DUST); do \
	!( test -e $$dustfile ) || rm $$dustfile; \
	done
	cd figures/tikz/; make clean

clean-all : clean
	for dd in $(srcdir); do \
	cd $$dd && make clean-all; cd ..;\
	done
	cd figures/tikz/; make clean-all
