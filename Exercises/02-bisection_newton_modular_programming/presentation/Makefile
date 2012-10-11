
tex = $(wildcard *.tex)
pdf = $(tex:.tex=.pdf)

.phony: all clean clean-all

all: $(pdf)

%.pdf: %.tex
	pdflatex $^
	pdflatex $^

clean:
	rm -f *.aux *.log *.out *.nav *.snm *toc *.vrb *~

clean-all: clean
	rm -f *.pdf
