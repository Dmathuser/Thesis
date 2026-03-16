#include <iostream>
#include <iomanip>
#include <cstring>
#include <sys/stat.h>
#include "simulation.h"
#include "simLogger.h"
#include "randWalk.h"
#include "PIG.h"
#include "EPIG.h"
#include "TPIG.h"
#include "ETPIG.h"

using namespace std;

StateTransition getSAS(State s, Action a, Simulation *sim);

void runTrial(string simFile, string outDir, string policy,
              int episodeLength, int epsilon, int seed)
{
    Simulation sim = Simulation(simFile, seed);
    string outFile;
    Policy *pol;
    int nS, nA;

    nS = sim.getNumStates();
    nA = sim.getNumActions();
    outFile = outDir + "/" + policy + "_" + to_string(seed) + ".txt";

    RandWalk randPol(seed, nS, nA);
    PIG_alg pigPol(seed, nS, nA);
    EPIG_alg epigPol(seed, epsilon, nS, nA);
    TPIG_alg tpigPol(seed, nS, nA);
    ETPIG_alg etpigPol(seed, epsilon, nS, nA);

    pol = NULL;
    if (policy == "RandWalk")
        pol = &randPol;
    else if (policy == "PIG")
        pol = &pigPol;
    else if (policy == "EPIG")
        pol = &epigPol;
    else if (policy == "TPIG")
        pol = &tpigPol;
    else if (policy == "ETPIG")
        pol = &etpigPol;

    if (pol == NULL) {
        cerr << "Unknown policy: " << policy << endl;
        return;
    }

    SimLogger logger = SimLogger(&sim, pol, simFile, sim.getSeed());

    sim.setCurState(5);
    State s = {5, 0};
    Action a = UP;
    StateTransition sas = {0};

    logger.startLog();
    int i;
    for (i = 0; i < episodeLength; i++) {
        logger.log();
        a = pol->getAction(s);
        sim.moveState(a);
        sas = getSAS(s, a, &sim);
        pol->Update(sas);
        s = sas.sPrime;
    }
    logger.stopLog();
    logger.printSimLogs(outFile);
}

StateTransition getSAS(State s, Action a, Simulation *sim)
{
    StateTransition sas = {0};
    sas.s = s;
    sas.a = a;
    sas.sPrime.StateId = sim->getCurStateIndex();
    if (sim->isNoisyMove(s, a))
        sas.sPrime.Noise = sim->getNoiseCount();
    else
        sas.sPrime.Noise = 0;
    return sas;
}

int main(int argc, char **argv)
{
    string simFiles[] = {"Simulation_4TV.sim", "Simulation_1TV.sim", "NoTvSim.sim"};
    string simNames[] = {"4TV", "1TV", "NoTV"};
    string policies[] = {"RandWalk", "PIG", "EPIG", "TPIG", "ETPIG"};
    int numSims = 3;
    int numPolicies = 5;
    int numTrials = 100;
    int episodeLength = 1000;
    int epsilon = 10;
    string baseDir = "experiment_data";
    int si, pi, trial;

    mkdir(baseDir.c_str(), 0755);

    for (si = 0; si < numSims; si++) {
        for (pi = 0; pi < numPolicies; pi++) {
            string dir = baseDir + "/" + simNames[si] + "/" + policies[pi];
            string parentDir = baseDir + "/" + simNames[si];
            mkdir(parentDir.c_str(), 0755);
            mkdir(dir.c_str(), 0755);

            cout << "Running " << policies[pi] << " on " << simNames[si]
                 << " (" << numTrials << " trials)..." << endl;

            for (trial = 0; trial < numTrials; trial++) {
                if ((trial + 1) % 25 == 0)
                    cout << "  Trial " << trial + 1 << "/" << numTrials << endl;
                runTrial(simFiles[si], dir, policies[pi],
                         episodeLength, epsilon, trial);
            }
        }
    }

    cout << "All experiments complete. Data in " << baseDir << "/" << endl;
    return 0;
}
