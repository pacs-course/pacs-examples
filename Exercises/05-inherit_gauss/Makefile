rerun = "There were undefined references"
bibtex = "\citation"

doc = text
dep   = $(wildcard *.tex) $(src) $(fig)
bib = ../common/bibliography.bib

src = $(wildcard ./es*/*.cpp) $(wildcard ./es*/*.hpp)

fig = $(wildcard ./figures/eps/*.eps) $(wildcard ./figures/fig/*.fig)

DUST = *.aux *.log *.nav *.out *.dvi *.pdf *.ps *.snm *.toc *.bbl *.blg *.vrb *.txt

vpath %.eps ./figures/eps/
vpath %.fig ./figures/fig/
vpath %.hpp ./es1:./es2
vpath %.cpp ./es1:./es2

.PHONY = default clean

dvi : $(doc).dvi

ps : $(doc).ps

pdf : $(doc).pdf

%.dvi : %.tex $(dep)
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

clean-all : clean
	for dd in $(srcdir); do \
	cd $$dd && make clean-all; cd ..;\
	done
