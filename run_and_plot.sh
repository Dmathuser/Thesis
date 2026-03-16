#!/bin/bash
# Run all experiments and generate plots with error bars.
# Usage: ./run_and_plot.sh
#
# This script:
# 1. Compiles run_experiments.cpp
# 2. Runs all 5 policies on all 3 simulations (100 trials each)
# 3. Generates PDF plots with mean +/- 1 std dev shading
# 4. Copies the new figures into FLAIRS/images_errbar/

set -e

cd "$(dirname "$0")"

echo "=== Step 1: Compiling simulator ==="
make
echo ""

echo "=== Step 2: Compiling experiment runner ==="
g++ -Wall -I. -O2 -o run_experiments run_experiments.cpp \
    simulation.o simLogger.o printSimPDF.o policy.o \
    randWalk.o PIG.o EPIG.o TPIG.o ETPIG.o structs.o
echo "Compiled run_experiments"
echo ""

echo "=== Step 3: Running experiments ==="
echo "  5 policies x 3 simulations x 100 trials x 1000 timesteps"
./run_experiments
echo ""

echo "=== Step 4: Generating plots with error bars ==="
python3 plot_results.py
echo ""

echo "=== Done ==="
echo "New figures with error bars are in FLAIRS/images_errbar/"
echo "To use them in the paper, update the image paths in FLAIRS/FLAIRS.tex"
