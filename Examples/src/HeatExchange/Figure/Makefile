SUFFIXES=.pdf .eps .tex .dia .fig .svg

FIGFIGURES=${wildcard *.fig}
DIAFIGURES=${wildcard *.dia}
SVGFIGURES=${wildcard *.svg}
PNGFIGURES=${wildcard *.png}
FIGEPS=$(DIAFIGURES:.dia=.eps)
FIGEPS+=$(FIGFIGURES:.fig=.eps)
FIGEPS+=$(SVGFIGURES:.svg=.eps)
FIGEPS+=$(PNGFIGURES:.png=.eps)
FIGPDF=$(FIGFIGURES:.fig=.pdf)
FIGPDF+=$(DIAFIGURES:.dia=.pdf)
FIGPDF+=$(SVGFIGURES:.svg=.pdf)
FIGPDF+=$(PNGFIGURES:.png=.pdf)
all: ps pdf

ps: $(FIGEPS)

pdf: $(FIGPDF)


clean:
	for f in *.fig; do rm -f `basename $$f '.fig'`.eps; done
	for f in *.fig; do rm -f `basename $$f '.fig'`.pdf; done
	for f in *.svg; do rm -f `basename $$f '.svg'`.pdf; done
	for f in *.svg; do rm -f `basename $$f '.svg'`.eps; done
	for f in *.dia; do rm -f `basename $$f '.dia'`.eps; done
	for f in *.dia; do rm -f `basename $$f '.dia'`.pdf; done

%.eps:%.fig
	fig2dev -L eps $< $@

%.eps:%.dia
	dia -n --export=$@ $<


%.eps:%.png
	convert  $< $@

%.pdf:%.fig
	fig2dev -L pdf $< $@

%.pdf:%.png
	convert $< $@

%.eps:%.svg
	inkscape -D --file=$<  --export-eps=$@

%.pdf:%.svg
	inkscape -D --file=$<  --export-pdf=$@

%.pdf:%.eps
	epstopdf --outfile=$@ $< 
