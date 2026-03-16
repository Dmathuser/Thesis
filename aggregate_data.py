#!/usr/bin/env python3
"""
Aggregate per-trial experiment data into summary files (mean +/- std)
for gnuplot consumption.  One output file per (simulation, metric).
"""

import os
import numpy as np

DATA_DIR = "experiment_data"
OUT_DIR = "experiment_summary"

SIMS = ["4TV", "1TV", "NoTV"]
SIM_LABELS = {"4TV": "4-TV", "1TV": "1-TV", "NoTV": "No-TV"}
POLICIES = ["RandWalk", "PIG", "EPIG", "TPIG", "ETPIG"]

COL_DISTRACTION = 1
COL_MISSED = 2
COL_ACCURACY = 3

METRICS = {
    "Distraction_Rate": COL_DISTRACTION,
    "Missed_States": COL_MISSED,
    "Model_Accuracy": COL_ACCURACY,
}


def load_trials(sim_name, policy_name, col_idx):
    trial_dir = os.path.join(DATA_DIR, sim_name, policy_name)
    files = sorted([f for f in os.listdir(trial_dir) if f.endswith('.txt')])
    all_data = []
    for fname in files:
        path = os.path.join(trial_dir, fname)
        vals = []
        with open(path, 'r') as f:
            lines = f.readlines()
        header_end = 0
        for i, line in enumerate(lines):
            if line.startswith("TimeStep"):
                header_end = i + 1
        for line in lines[header_end:]:
            line = line.strip()
            if not line:
                continue
            parts = line.split(',')
            vals.append(float(parts[col_idx]))
        all_data.append(vals)
    return np.array(all_data)


def main():
    os.makedirs(OUT_DIR, exist_ok=True)

    for sim in SIMS:
        sim_label = SIM_LABELS[sim]
        for metric_name, col_idx in METRICS.items():
            outfile = os.path.join(OUT_DIR,
                                   f"{metric_name}_{sim_label}.dat")
            # Load all policies
            policy_data = {}
            for pol in POLICIES:
                data = load_trials(sim, pol, col_idx)
                policy_data[pol] = {
                    'mean': np.mean(data, axis=0),
                    'std': np.std(data, axis=0),
                }

            n_steps = len(policy_data[POLICIES[0]]['mean'])

            with open(outfile, 'w') as f:
                # Header
                cols = ["TimeStep"]
                for pol in POLICIES:
                    cols.append(f"{pol}_mean")
                    cols.append(f"{pol}_lo")
                    cols.append(f"{pol}_hi")
                f.write("# " + "  ".join(cols) + "\n")

                for t in range(n_steps):
                    row = [str(t)]
                    for pol in POLICIES:
                        m = policy_data[pol]['mean'][t]
                        s = policy_data[pol]['std'][t]
                        row.append(f"{m:.6f}")
                        row.append(f"{m - s:.6f}")
                        row.append(f"{m + s:.6f}")
                    f.write("  ".join(row) + "\n")

            print(f"  Wrote {outfile}")


if __name__ == '__main__':
    main()
