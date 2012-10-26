rerun = "There were undefined references"
bibtex = "\citation"

doc = es4
dep = $(wildcard *.tex) $(srcdir)/$(src)
bib = ../common/bibliography.bib

srcdir = ./src/yang ./src/yang/src
src = $(wildcard *.cpp) $(wildcard *.hpp)

DUST = *.aux *.log *.nav *.out *.dvi *.pdf *.ps *.snm *.toc *.bbl *.blg *.vrb *.txt *~

.PHONY = all pdf dvi ps figures clean

all: pdf

dvi : $(doc).dvi

ps : $(doc).ps

pdf : figures $(doc).pdf

figures:
	cd fig && make

%.dvi : %.tex $(bib) $(dep)
	latex $*
# 	if (egrep -q $(bibtex) $*.aux); then bibtex $* && latex $*; fi
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
