TARGET=lab_01.pdf
TEXINPUTS=./:../LATEX:../figures:

all: $(TARGET)

%.pdf: *.tex
	TEXINPUTS=$(TEXINPUTS) pdflatex $*
	TEXINPUTS=$(TEXINPUTS) pdflatex $*
	TEXINPUTS=$(TEXINPUTS) pdflatex $*

clean:
	$(RM) *.aux *.log *.toc *~ *.blg *.out *.bbl \
	*.nav *.vrb *.snm

distclean: clean
	$(RM) $(TARGET)
