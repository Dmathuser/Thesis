# Generate all 9 figures for the FLAIRS paper.
# Usage: gnuplot plot_all.gp
# Then run inline_gnuplot_figs.py to embed into FLAIRS.tex

DATADIR = "../../experiment_summary"

# Legend entries use \scriptsize via LaTeX
RW  = "\\scriptsize Random Walk"
P   = "\\scriptsize PIG"
EP  = "\\scriptsize EPIG"
TP  = "\\scriptsize TPIG"
ETP = "\\scriptsize ETPIG"
LP  = "\\scriptsize LP"

# ---- Distraction Rate: 4-TV ----
load "common.gp"
set output "Distraction_Rate_4-TV.tex"
set ylabel "\\footnotesize Cumulative Distractions"
set title "\\small Distraction Rate: 4-TV Simulation"

plot \
  DATADIR."/Distraction_Rate_4-TV.dat" using 1:3:4 with filledcurves linecolor rgb "gray50" notitle, \
  "" using 1:6:7 with filledcurves linecolor rgb "gray30" notitle, \
  "" using 1:9:10 with filledcurves linecolor rgb "gray40" notitle, \
  "" using 1:12:13 with filledcurves linecolor rgb "gray20" notitle, \
  "" using 1:15:16 with filledcurves linecolor rgb "gray35" notitle, \
  "" using 1:2 with lines linestyle 1 title RW, \
  "" using 1:5 with lines linestyle 2 title P, \
  "" using 1:8 with lines linestyle 3 title EP, \
  "" using 1:11 with lines linestyle 4 title TP, \
  "" using 1:18:19 with filledcurves linecolor rgb "gray25" notitle, \
  "" using 1:14 with lines linestyle 5 title ETP, \
  "" using 1:17 with lines linestyle 6 title LP

# ---- Distraction Rate: 1-TV ----
load "common.gp"
set output "Distraction_Rate_1-TV.tex"
set ylabel "\\footnotesize Cumulative Distractions"
set title "\\small Distraction Rate: 1-TV Simulation"

plot \
  DATADIR."/Distraction_Rate_1-TV.dat" using 1:3:4 with filledcurves linecolor rgb "gray50" notitle, \
  "" using 1:6:7 with filledcurves linecolor rgb "gray30" notitle, \
  "" using 1:9:10 with filledcurves linecolor rgb "gray40" notitle, \
  "" using 1:12:13 with filledcurves linecolor rgb "gray20" notitle, \
  "" using 1:15:16 with filledcurves linecolor rgb "gray35" notitle, \
  "" using 1:2 with lines linestyle 1 title RW, \
  "" using 1:5 with lines linestyle 2 title P, \
  "" using 1:8 with lines linestyle 3 title EP, \
  "" using 1:11 with lines linestyle 4 title TP, \
  "" using 1:18:19 with filledcurves linecolor rgb "gray25" notitle, \
  "" using 1:14 with lines linestyle 5 title ETP, \
  "" using 1:17 with lines linestyle 6 title LP

# ---- Distraction Rate: No-TV ----
load "common.gp"
set output "Distraction_Rate_No-TV.tex"
set ylabel "\\footnotesize Cumulative Distractions"
set title "\\small Distraction Rate: No-TV Simulation"

plot \
  DATADIR."/Distraction_Rate_No-TV.dat" using 1:3:4 with filledcurves linecolor rgb "gray50" notitle, \
  "" using 1:6:7 with filledcurves linecolor rgb "gray30" notitle, \
  "" using 1:9:10 with filledcurves linecolor rgb "gray40" notitle, \
  "" using 1:12:13 with filledcurves linecolor rgb "gray20" notitle, \
  "" using 1:15:16 with filledcurves linecolor rgb "gray35" notitle, \
  "" using 1:2 with lines linestyle 1 title RW, \
  "" using 1:5 with lines linestyle 2 title P, \
  "" using 1:8 with lines linestyle 3 title EP, \
  "" using 1:11 with lines linestyle 4 title TP, \
  "" using 1:18:19 with filledcurves linecolor rgb "gray25" notitle, \
  "" using 1:14 with lines linestyle 5 title ETP, \
  "" using 1:17 with lines linestyle 6 title LP

# ---- Missed States: 4-TV ----
load "common.gp"
set output "Missed_States_4-TV.tex"
set ylabel "\\footnotesize Undiscovered State Transitions"
set title "\\small Missed State Transitions: 4-TV Simulation"

plot \
  DATADIR."/Missed_States_4-TV.dat" using 1:3:4 with filledcurves linecolor rgb "gray50" notitle, \
  "" using 1:6:7 with filledcurves linecolor rgb "gray30" notitle, \
  "" using 1:9:10 with filledcurves linecolor rgb "gray40" notitle, \
  "" using 1:12:13 with filledcurves linecolor rgb "gray20" notitle, \
  "" using 1:15:16 with filledcurves linecolor rgb "gray35" notitle, \
  "" using 1:2 with lines linestyle 1 title RW, \
  "" using 1:5 with lines linestyle 2 title P, \
  "" using 1:8 with lines linestyle 3 title EP, \
  "" using 1:11 with lines linestyle 4 title TP, \
  "" using 1:18:19 with filledcurves linecolor rgb "gray25" notitle, \
  "" using 1:14 with lines linestyle 5 title ETP, \
  "" using 1:17 with lines linestyle 6 title LP

# ---- Missed States: 1-TV ----
load "common.gp"
set output "Missed_States_1-TV.tex"
set ylabel "\\footnotesize Undiscovered State Transitions"
set title "\\small Missed State Transitions: 1-TV Simulation"

plot \
  DATADIR."/Missed_States_1-TV.dat" using 1:3:4 with filledcurves linecolor rgb "gray50" notitle, \
  "" using 1:6:7 with filledcurves linecolor rgb "gray30" notitle, \
  "" using 1:9:10 with filledcurves linecolor rgb "gray40" notitle, \
  "" using 1:12:13 with filledcurves linecolor rgb "gray20" notitle, \
  "" using 1:15:16 with filledcurves linecolor rgb "gray35" notitle, \
  "" using 1:2 with lines linestyle 1 title RW, \
  "" using 1:5 with lines linestyle 2 title P, \
  "" using 1:8 with lines linestyle 3 title EP, \
  "" using 1:11 with lines linestyle 4 title TP, \
  "" using 1:18:19 with filledcurves linecolor rgb "gray25" notitle, \
  "" using 1:14 with lines linestyle 5 title ETP, \
  "" using 1:17 with lines linestyle 6 title LP

# ---- Missed States: No-TV ----
load "common.gp"
set output "Missed_States_No-TV.tex"
set ylabel "\\footnotesize Undiscovered State Transitions"
set title "\\small Missed State Transitions: No-TV Simulation"

plot \
  DATADIR."/Missed_States_No-TV.dat" using 1:3:4 with filledcurves linecolor rgb "gray50" notitle, \
  "" using 1:6:7 with filledcurves linecolor rgb "gray30" notitle, \
  "" using 1:9:10 with filledcurves linecolor rgb "gray40" notitle, \
  "" using 1:12:13 with filledcurves linecolor rgb "gray20" notitle, \
  "" using 1:15:16 with filledcurves linecolor rgb "gray35" notitle, \
  "" using 1:2 with lines linestyle 1 title RW, \
  "" using 1:5 with lines linestyle 2 title P, \
  "" using 1:8 with lines linestyle 3 title EP, \
  "" using 1:11 with lines linestyle 4 title TP, \
  "" using 1:18:19 with filledcurves linecolor rgb "gray25" notitle, \
  "" using 1:14 with lines linestyle 5 title ETP, \
  "" using 1:17 with lines linestyle 6 title LP

# ---- Model Accuracy: 4-TV ----
load "common.gp"
set output "Model_Accuracy_4-TV.tex"
set ylabel "\\footnotesize KL-Divergence"
set title "\\small Model Accuracy: 4-TV Simulation"

plot \
  DATADIR."/Model_Accuracy_4-TV.dat" using 1:3:4 with filledcurves linecolor rgb "gray50" notitle, \
  "" using 1:6:7 with filledcurves linecolor rgb "gray30" notitle, \
  "" using 1:9:10 with filledcurves linecolor rgb "gray40" notitle, \
  "" using 1:12:13 with filledcurves linecolor rgb "gray20" notitle, \
  "" using 1:15:16 with filledcurves linecolor rgb "gray35" notitle, \
  "" using 1:2 with lines linestyle 1 title RW, \
  "" using 1:5 with lines linestyle 2 title P, \
  "" using 1:8 with lines linestyle 3 title EP, \
  "" using 1:11 with lines linestyle 4 title TP, \
  "" using 1:18:19 with filledcurves linecolor rgb "gray25" notitle, \
  "" using 1:14 with lines linestyle 5 title ETP, \
  "" using 1:17 with lines linestyle 6 title LP

# ---- Model Accuracy: 1-TV ----
load "common.gp"
set output "Model_Accuracy_1-TV.tex"
set ylabel "\\footnotesize KL-Divergence"
set title "\\small Model Accuracy: 1-TV Simulation"

plot \
  DATADIR."/Model_Accuracy_1-TV.dat" using 1:3:4 with filledcurves linecolor rgb "gray50" notitle, \
  "" using 1:6:7 with filledcurves linecolor rgb "gray30" notitle, \
  "" using 1:9:10 with filledcurves linecolor rgb "gray40" notitle, \
  "" using 1:12:13 with filledcurves linecolor rgb "gray20" notitle, \
  "" using 1:15:16 with filledcurves linecolor rgb "gray35" notitle, \
  "" using 1:2 with lines linestyle 1 title RW, \
  "" using 1:5 with lines linestyle 2 title P, \
  "" using 1:8 with lines linestyle 3 title EP, \
  "" using 1:11 with lines linestyle 4 title TP, \
  "" using 1:18:19 with filledcurves linecolor rgb "gray25" notitle, \
  "" using 1:14 with lines linestyle 5 title ETP, \
  "" using 1:17 with lines linestyle 6 title LP

# ---- Model Accuracy: No-TV ----
load "common.gp"
set output "Model_Accuracy_No-TV.tex"
set ylabel "\\footnotesize KL-Divergence"
set title "\\small Model Accuracy: No-TV Simulation"

plot \
  DATADIR."/Model_Accuracy_No-TV.dat" using 1:3:4 with filledcurves linecolor rgb "gray50" notitle, \
  "" using 1:6:7 with filledcurves linecolor rgb "gray30" notitle, \
  "" using 1:9:10 with filledcurves linecolor rgb "gray40" notitle, \
  "" using 1:12:13 with filledcurves linecolor rgb "gray20" notitle, \
  "" using 1:15:16 with filledcurves linecolor rgb "gray35" notitle, \
  "" using 1:2 with lines linestyle 1 title RW, \
  "" using 1:5 with lines linestyle 2 title P, \
  "" using 1:8 with lines linestyle 3 title EP, \
  "" using 1:11 with lines linestyle 4 title TP, \
  "" using 1:18:19 with filledcurves linecolor rgb "gray25" notitle, \
  "" using 1:14 with lines linestyle 5 title ETP, \
  "" using 1:17 with lines linestyle 6 title LP

# ---- Distraction Rate: 10x10-2TV ----
load "common.gp"
set output "Distraction_Rate_10x10-2TV.tex"
set ylabel "\\footnotesize Cumulative Distractions"
set title "\\small Distraction Rate: 10$\\times$10 2-TV Simulation"

plot \
  DATADIR."/Distraction_Rate_10x10-2TV.dat" using 1:3:4 with filledcurves linecolor rgb "gray50" notitle, \
  "" using 1:6:7 with filledcurves linecolor rgb "gray30" notitle, \
  "" using 1:9:10 with filledcurves linecolor rgb "gray40" notitle, \
  "" using 1:12:13 with filledcurves linecolor rgb "gray20" notitle, \
  "" using 1:15:16 with filledcurves linecolor rgb "gray35" notitle, \
  "" using 1:2 with lines linestyle 1 title RW, \
  "" using 1:5 with lines linestyle 2 title P, \
  "" using 1:8 with lines linestyle 3 title EP, \
  "" using 1:11 with lines linestyle 4 title TP, \
  "" using 1:18:19 with filledcurves linecolor rgb "gray25" notitle, \
  "" using 1:14 with lines linestyle 5 title ETP, \
  "" using 1:17 with lines linestyle 6 title LP

# ---- Missed States: 10x10-2TV ----
load "common.gp"
set output "Missed_States_10x10-2TV.tex"
set ylabel "\\footnotesize Undiscovered State Transitions"
set title "\\small Missed State Transitions: 10$\\times$10 2-TV Simulation"

plot \
  DATADIR."/Missed_States_10x10-2TV.dat" using 1:3:4 with filledcurves linecolor rgb "gray50" notitle, \
  "" using 1:6:7 with filledcurves linecolor rgb "gray30" notitle, \
  "" using 1:9:10 with filledcurves linecolor rgb "gray40" notitle, \
  "" using 1:12:13 with filledcurves linecolor rgb "gray20" notitle, \
  "" using 1:15:16 with filledcurves linecolor rgb "gray35" notitle, \
  "" using 1:2 with lines linestyle 1 title RW, \
  "" using 1:5 with lines linestyle 2 title P, \
  "" using 1:8 with lines linestyle 3 title EP, \
  "" using 1:11 with lines linestyle 4 title TP, \
  "" using 1:18:19 with filledcurves linecolor rgb "gray25" notitle, \
  "" using 1:14 with lines linestyle 5 title ETP, \
  "" using 1:17 with lines linestyle 6 title LP

# ---- Model Accuracy: 10x10-2TV ----
load "common.gp"
set output "Model_Accuracy_10x10-2TV.tex"
set ylabel "\\footnotesize KL-Divergence"
set title "\\small Model Accuracy: 10$\\times$10 2-TV Simulation"

plot \
  DATADIR."/Model_Accuracy_10x10-2TV.dat" using 1:3:4 with filledcurves linecolor rgb "gray50" notitle, \
  "" using 1:6:7 with filledcurves linecolor rgb "gray30" notitle, \
  "" using 1:9:10 with filledcurves linecolor rgb "gray40" notitle, \
  "" using 1:12:13 with filledcurves linecolor rgb "gray20" notitle, \
  "" using 1:15:16 with filledcurves linecolor rgb "gray35" notitle, \
  "" using 1:2 with lines linestyle 1 title RW, \
  "" using 1:5 with lines linestyle 2 title P, \
  "" using 1:8 with lines linestyle 3 title EP, \
  "" using 1:11 with lines linestyle 4 title TP, \
  "" using 1:18:19 with filledcurves linecolor rgb "gray25" notitle, \
  "" using 1:14 with lines linestyle 5 title ETP, \
  "" using 1:17 with lines linestyle 6 title LP
