rerun = "There were undefined references"
bibtex = "\citation"

doc = es8
dep   = $(wildcard *.tex) $(srcdir)/$(src)
bib = ../common/bibliography.bib

srcdir = ./es/2 ./es/2/src ./es/1/inherit1 ./es/1/inherit2 ./es/1/template
src = $(wildcard *.cpp) $(wildcard *.hpp)

DUST = *.aux *.log *.nav *.out *.dvi *.pdf *.ps *.snm *.toc *.bbl *.blg *.vrb *.txt

.PHONY : dvi ps pdf clean

dvi : $(doc).dvi

ps : $(doc).ps

pdf : $(doc).pdf

%.dvi : %.tex $(bib) $(dep)
	latex $*
	if (egrep -q $(bibtex) $*.aux); then bibtex $* && latex $*; fi
	latex $* && !(egrep -q $(rerun) $*.log)

%.ps : %.dvi
	dvips $< -o $*.ps

%.pdf : %.ps
	ps2pdf $*.ps

clean :
	for dustfile in $(DUST); do \
	!( test -e $$dustfile ) || rm $$dustfile; \
	done
