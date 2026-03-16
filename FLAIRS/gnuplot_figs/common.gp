# Common gnuplot settings for all FLAIRS paper figures.
# B&W, epslatex terminal, Times fonts via LaTeX.

set terminal cairolatex pdf size 3.3in,2.5in font ",6"

set border linewidth 1.0
set grid linewidth 0.3 linecolor rgb "gray70"
set key top right spacing 1.4 font ",5"
set xlabel "\\footnotesize TimeStep"
set ylabel "" # set per-plot
set tics font ",5"
set format x "\\scriptsize %g"
set format y "\\scriptsize %g"

# Line styles: distinct dash patterns, all black/gray
# RandWalk: thin gray solid
set style line 1 linecolor rgb "gray50" linewidth 1.5 dashtype 1
# PIG: black dashed
set style line 2 linecolor rgb "black"  linewidth 1.5 dashtype 2
# EPIG: black dotted
set style line 3 linecolor rgb "black"  linewidth 1.5 dashtype 3
# TPIG: thick black solid
set style line 4 linecolor rgb "black"  linewidth 2.5 dashtype 1
# ETPIG: black dash-dot
set style line 5 linecolor rgb "black"  linewidth 1.5 dashtype 5

# Fill styles for error bands (light grays, very transparent)
set style fill transparent solid 0.12 noborder
