rerun = "There were undefined references"
bibtex = "\citation"

doc = es6
dep   = $(wildcard *.tex) $(src) $(fig)
bib = ../common/bibliography.bib

src = $(wildcard ./es*/*.cpp) $(wildcard ./es*/*.hpp)


DUST = *.aux *.log *.nav *.out *.dvi *.pdf *.ps *.snm *.toc *.bbl *.blg *.vrb *.txt *~

.PHONY = all pdf dvi ps clean clean-all

all: pdf

dvi : $(doc).dvi

ps : $(doc).ps

pdf : $(doc).pdf

%.dvi : %.tex $(dep)
	latex $*
	if (egrep -q $(bibtex) $*.aux); then bibtex $* && latex $*; fi
	latex $* && !(egrep -q $(rerun) $*.log)

%.ps : %.dvi
	dvips $< -o $*.ps

%.pdf : %.tex $(dep)
	pdflatex $*
# 	if (egrep -q $(bibtex) $*.aux); then bibtex $* && pdflatex $*; fi
	pdflatex $* && !(egrep -q $(rerun) $*.log)

clean :
	for dustfile in $(DUST); do \
	!( test -e $$dustfile ) || rm $$dustfile; \
	done

clean-all : clean
	for dd in $(srcdir); do \
	cd $$dd && make clean-all; cd ..;\
	done
