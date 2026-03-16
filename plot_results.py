#!/usr/bin/env python3
"""
Plot experiment results with error bars (mean +/- 1 std dev shaded region).
Reads per-trial CSV data from experiment_data/ and generates PDF figures
suitable for the FLAIRS paper.
"""

import os
import sys
import numpy as np
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt

DATA_DIR = "experiment_data"
OUT_DIR = "FLAIRS/images_errbar"

SIMS = ["4TV", "1TV", "NoTV"]
SIM_LABELS = {"4TV": "4-TV", "1TV": "1-TV", "NoTV": "No-TV"}
POLICIES = ["RandWalk", "PIG", "EPIG", "TPIG", "ETPIG"]
POLICY_LABELS = {
    "RandWalk": "Random Walk",
    "PIG": "PIG",
    "EPIG": "EPIG",
    "TPIG": "TPIG",
    "ETPIG": "ETPIG"
}
POLICY_COLORS = {
    "RandWalk": "#1f77b4",
    "PIG": "#ff7f0e",
    "EPIG": "#2ca02c",
    "TPIG": "#d62728",
    "ETPIG": "#9467bd"
}

# Column indices in the CSV (after the 3 header lines)
COL_TIMESTEP = 0
COL_DISTRACTION = 1
COL_MISSED = 2
COL_ACCURACY = 3


def load_trials(sim_name, policy_name):
    """Load all trial data for a given simulation and policy.
    Returns a dict of numpy arrays, one per metric, shape (n_trials, n_timesteps).
    """
    trial_dir = os.path.join(DATA_DIR, sim_name, policy_name)
    files = sorted([f for f in os.listdir(trial_dir)
                    if f.endswith('.txt')])

    all_distraction = []
    all_missed = []
    all_accuracy = []

    for fname in files:
        path = os.path.join(trial_dir, fname)
        distraction = []
        missed = []
        accuracy = []
        with open(path, 'r') as f:
            lines = f.readlines()

        # Skip header lines (Seed, Number of samples, Simulation name, column header)
        header_end = 0
        for i, line in enumerate(lines):
            if line.startswith("TimeStep"):
                header_end = i + 1

        for line in lines[header_end:]:
            line = line.strip()
            if not line:
                continue
            parts = line.split(',')
            distraction.append(float(parts[COL_DISTRACTION]))
            missed.append(float(parts[COL_MISSED]))
            accuracy.append(float(parts[COL_ACCURACY]))

        all_distraction.append(distraction)
        all_missed.append(missed)
        all_accuracy.append(accuracy)

    return {
        'distraction': np.array(all_distraction),
        'missed': np.array(all_missed),
        'accuracy': np.array(all_accuracy),
    }


def plot_metric(sim_name, metric_key, ylabel, title, out_filename,
                all_data):
    """Plot one metric for one simulation with mean line and std shading."""
    fig, ax = plt.subplots(figsize=(6, 4))

    for policy in POLICIES:
        data = all_data[policy][metric_key]
        mean = np.mean(data, axis=0)
        std = np.std(data, axis=0)
        timesteps = np.arange(len(mean))

        color = POLICY_COLORS[policy]
        label = POLICY_LABELS[policy]
        ax.plot(timesteps, mean, color=color, label=label, linewidth=1.5)
        ax.fill_between(timesteps, mean - std, mean + std,
                        color=color, alpha=0.15)

    ax.set_xlabel("TimeStep", fontsize=11)
    ax.set_ylabel(ylabel, fontsize=11)
    ax.set_title(title, fontsize=12)
    ax.legend(fontsize=9, loc='best')
    ax.grid(True, alpha=0.3)
    fig.tight_layout()
    fig.savefig(out_filename, dpi=300, bbox_inches='tight')
    plt.close(fig)
    print(f"  Saved {out_filename}")


def main():
    os.makedirs(OUT_DIR, exist_ok=True)

    for sim in SIMS:
        sim_label = SIM_LABELS[sim]
        print(f"Processing {sim_label}...")

        # Load all policy data for this simulation
        all_data = {}
        for policy in POLICIES:
            all_data[policy] = load_trials(sim, policy)

        # Distraction Rate
        plot_metric(
            sim, 'distraction',
            "Cumulative Distractions",
            f"Distraction Rate: {sim_label} Simulation",
            os.path.join(OUT_DIR, f"Distraction_Rate_{sim_label}.pdf"),
            all_data
        )

        # Missed States (Learning Rate)
        plot_metric(
            sim, 'missed',
            "Undiscovered State Transitions",
            f"Missed State Transitions: {sim_label} Simulation",
            os.path.join(OUT_DIR, f"Missed_States_{sim_label}.pdf"),
            all_data
        )

        # Model Accuracy
        plot_metric(
            sim, 'accuracy',
            "KL-Divergence",
            f"Model Accuracy: {sim_label} Simulation",
            os.path.join(OUT_DIR, f"Model_Accuracy_{sim_label}.pdf"),
            all_data
        )

    print("\nDone. Figures saved to", OUT_DIR)


if __name__ == '__main__':
    main()
