CSRCDIR= ..

CSRC= $(addprefix $(CSRCDIR)/,speed_run.c)

TEXSRC= relatorio.tex

ENG= pdflatex

all: clean pdf

pdf: $(TEXSRC) $(CSRC)
	$(ENG) -shell-escape $(TEXSRC)
	touch pdf

clean:
	rm -f *.toc *.lof *.aux *.blg *.out *.xml *.bbl *.log *-blx.bib
