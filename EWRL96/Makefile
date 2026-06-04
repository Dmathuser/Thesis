PAPER = EWRL
BIB = Thesis_David_Mathews
GPDIR = gnuplot_figs_ewrl
STAMP = $(GPDIR)/.gnuplot.stamp

all: $(PAPER).pdf

# Delegate figure rebuilds to the sub-make. The sub-make handles all
# dependency tracking and only touches the stamp when figures actually
# changed.
.PHONY: figures
figures:
	$(MAKE) -C $(GPDIR)

# The stamp file is maintained by the sub-make above. Empty recipe ";"
# tells make this file is built by its (phony) prerequisite.
$(STAMP): figures ;

# EWRL.tex loads each gnuplot figure via \input{gnuplot_figs/<name>.tex},
# so the PDF depends on the gnuplot stamp being current. No inlining step
# is needed (the cairolatex .tex files are loaded at compile time, and
# \graphicspath in EWRL.tex resolves the bare-basename includegraphics
# calls to the gnuplot_figs/ directory).
$(PAPER).pdf: $(PAPER).tex $(BIB).bib $(STAMP)
	pdflatex $(PAPER)
	bibtex $(PAPER)
	pdflatex $(PAPER)
	pdflatex $(PAPER)

clean:
	rm -f $(PAPER).aux $(PAPER).bbl $(PAPER).blg $(PAPER).log $(PAPER).out
	$(MAKE) -C $(GPDIR) clean

distclean: clean
	rm -f $(PAPER).pdf

.PHONY: all clean distclean
