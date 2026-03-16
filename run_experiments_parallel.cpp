#include <iostream>
#include <iomanip>
#include <cstring>
#include <sys/stat.h>
#include <thread>
#include <mutex>
#include <queue>
#include "simulation.h"
#include "simLogger.h"
#include "randWalk.h"
#include "PIG.h"
#include "EPIG.h"
#include "TPIG.h"
#include "ETPIG.h"
#include "LP.h"

using namespace std;

StateTransition getSAS(State s, Action a, Simulation *sim);

struct Job {
    string simFile;
    string outDir;
    string policy;
    int episodeLength;
    int epsilon;
    int seed;
};

mutex queue_mutex;
mutex print_mutex;
int jobs_done = 0;
int jobs_total = 0;

void runTrial(const Job &job)
{
    Simulation sim = Simulation(job.simFile, job.seed);
    string outFile;
    int nS, nA;

    nS = sim.getNumStates();
    nA = sim.getNumActions();
    outFile = job.outDir + "/" + job.policy + "_" + to_string(job.seed) + ".txt";

    RandWalk randPol(job.seed, nS, nA);
    PIG_alg pigPol(job.seed, nS, nA);
    EPIG_alg epigPol(job.seed, job.epsilon, nS, nA);
    TPIG_alg tpigPol(job.seed, nS, nA);
    ETPIG_alg etpigPol(job.seed, job.epsilon, nS, nA);
    LP_alg lpPol(job.seed, nS, nA);

    Policy *pol;
    pol = NULL;
    if (job.policy == "RandWalk")
        pol = &randPol;
    else if (job.policy == "PIG")
        pol = &pigPol;
    else if (job.policy == "EPIG")
        pol = &epigPol;
    else if (job.policy == "TPIG")
        pol = &tpigPol;
    else if (job.policy == "ETPIG")
        pol = &etpigPol;
    else if (job.policy == "LP")
        pol = &lpPol;

    if (pol == NULL)
        return;

    SimLogger logger = SimLogger(&sim, pol, job.simFile, sim.getSeed());

    sim.setCurState(5);
    State s = {5, 0};
    Action a = UP;
    StateTransition sas = {0};

    logger.startLog();
    int i;
    for (i = 0; i < job.episodeLength; i++) {
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

queue<Job> jobQueue;

void worker()
{
    Job job;
    int done;

    for (;;) {
        {
            lock_guard<mutex> lock(queue_mutex);
            if (jobQueue.empty())
                return;
            job = jobQueue.front();
            jobQueue.pop();
        }

        runTrial(job);

        {
            lock_guard<mutex> lock(print_mutex);
            jobs_done++;
            done = jobs_done;
            if (done % 100 == 0 || done == jobs_total)
                cout << "  Progress: " << done << "/" << jobs_total << endl;
        }
    }
}

int main(int argc, char **argv)
{
    struct SimConfig {
        string file;
        string name;
        int episodeLength;
        int startState;
    };

    SimConfig sims[] = {
        {"Simulation_4TV.sim",       "4TV",       1000, 5},
        {"Simulation_1TV.sim",       "1TV",       1000, 5},
        {"NoTvSim.sim",              "NoTV",      1000, 5},
        {"Simulation_2TV_10x10.sim", "10x10_2TV", 3000, 5},
    };
    string policies[] = {"RandWalk", "PIG", "EPIG", "TPIG", "ETPIG", "LP"};
    int numSims = 4;
    int numPolicies = 6;
    int numTrials = 100;
    int epsilon = 10;
    int numThreads = 16;
    string baseDir = "experiment_data";
    int si, pi, trial;

    if (argc > 1)
        numThreads = atoi(argv[1]);

    mkdir(baseDir.c_str(), 0755);

    /* Build job queue */
    for (si = 0; si < numSims; si++) {
        for (pi = 0; pi < numPolicies; pi++) {
            string dir = baseDir + "/" + sims[si].name + "/" + policies[pi];
            string parentDir = baseDir + "/" + sims[si].name;
            mkdir(parentDir.c_str(), 0755);
            mkdir(dir.c_str(), 0755);

            for (trial = 0; trial < numTrials; trial++) {
                Job job;
                job.simFile = sims[si].file;
                job.outDir = dir;
                job.policy = policies[pi];
                job.episodeLength = sims[si].episodeLength;
                job.epsilon = epsilon;
                job.seed = trial;
                jobQueue.push(job);
            }
        }
    }

    jobs_total = jobQueue.size();
    cout << "Running " << jobs_total << " trials across "
         << numThreads << " threads..." << endl;

    /* Launch worker threads */
    vector<thread> threads;
    int t;
    for (t = 0; t < numThreads; t++)
        threads.push_back(thread(worker));

    for (t = 0; t < numThreads; t++)
        threads[t].join();

    cout << "All experiments complete. Data in " << baseDir << "/" << endl;
    return 0;
}
