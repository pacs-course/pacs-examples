
tex = $(wildcard *.tex)
pdf = $(tex:.tex=.pdf)

.phony: all clean clean-all

all: $(pdf)

%.pdf: %.tex
	pdflatex $^
	pdflatex $^

clean:
	rm -f *.aux *.log *~

clean-all: clean
	rm -f *.pdf
