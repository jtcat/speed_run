SRC= relatorio.tex

ENG= pdflatex

all: clean pdf

pdf: $(SRC)
	$(ENG) -shell-escape $(SRC)
	touch pdf

clean:
	rm -f *.toc *.lof *.aux *.blg *.out *.xml *.bbl *.log *-blx.bib
