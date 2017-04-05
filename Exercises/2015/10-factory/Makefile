rerun = "There were undefined references"
bibtex = "\citation"

doc = es10
dep = $(wildcard *.tex) $(srcdir)/$(src)
bib = ../common/bibliography.bib

srcdir = ./src
src = $(wildcard *.cpp) $(wildcard *.hpp)

DUST = *.aux *.log *.nav *.out *.dvi *.pdf *.ps *.snm *.toc *.bbl *.blg *.vrb *.txt *~

.PHONY = all pdf dvi ps tikz clean clean-all

all: pdf

dvi : $(doc).dvi

ps : $(doc).ps

pdf : $(doc).pdf

%.pdf : %.tex $(dep)
	pdflatex $*
	if (egrep -q $(bibtex) $*.aux); then bibtex $* && pdflatex $*; fi
	pdflatex $* && !(egrep -q $(rerun) $*.log)

%.dvi : %.tex $(bib) $(dep)
	latex $*
	if (egrep -q $(bibtex) $*.aux); then bibtex $* && latex $*; fi
	latex $* && !(egrep -q $(rerun) $*.log)

%.ps : %.dvi
	dvips $< -o $*.ps

clean :
	for dustfile in $(DUST); do \
	!( test -e $$dustfile ) || rm $$dustfile; \
	done

clean-all : clean
	for dd in $(srcdir); do \
	cd $$dd && make clean-all; cd ..;\
	done
