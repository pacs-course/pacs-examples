TARGET=lab_01.pdf

all: $(TARGET)


TEXINPUTS=./:../LATEX:../figures:

%.pdf: *.tex
	TEXINPUTS=$(TEXINPUTS) pdflatex $*
	#TEXINPUTS=$(TEXINPUTS) bibtex   $*
	TEXINPUTS=$(TEXINPUTS) pdflatex $*
	TEXINPUTS=$(TEXINPUTS) pdflatex $*

clean:
	$(RM) *.aux *.log *.toc *~ *.blg *.out *.bbl


distclean: clean
	$(RM) -f $(TARGET)
