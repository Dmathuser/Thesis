#include <iostream>
#include <cstdio>
#include <sys/stat.h>
#include <thread>
#include <mutex>
#include <queue>
#include "simulation.h"
#include "simLogger.h"
#include "TPIG.h"
#include "LP.h"

using namespace std;

StateTransition getSAS(State s, Action a, Simulation *sim);

struct Job {
    string simFile;
    string outDir;
    int episodeLength;
    int seed;
    double alpha;
    double gamma_val;
    string policy;
};

mutex queue_mutex;
mutex print_mutex;
queue<Job> jobQueue;
int jobs_done = 0;
int jobs_total = 0;

void runTrial(const Job &job)
{
    Simulation sim = Simulation(job.simFile, job.seed);
    int nS, nA;
    char fname[256];

    nS = sim.getNumStates();
    nA = sim.getNumActions();

    snprintf(fname, sizeof(fname), "%s/%s_a%.1f_g%.1f_%d.txt",
             job.outDir.c_str(), job.policy.c_str(),
             job.alpha, job.gamma_val, job.seed);

    Policy *pol;
    TPIG_alg tpigPol(job.seed, nS, nA);
    LP_alg lpPol(job.seed, nS, nA);

    pol = NULL;
    if (job.policy == "TPIG") {
        tpigPol.setParams(job.alpha, job.gamma_val);
        pol = &tpigPol;
    } else if (job.policy == "LP") {
        lpPol.setParams(job.alpha, job.gamma_val);
        pol = &lpPol;
    }

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
    logger.printSimLogs(string(fname));
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
    string simFile = "Simulation_1TV.sim";
    int episodeLength = 1000;
    int numTrials = 100;
    int numThreads = 16;
    string baseDir = "ablation_data";
    double alphas[] = {0.1, 0.5, 0.9};
    double gammas[] = {0.1, 0.5, 0.9};
    string policies[] = {"TPIG", "LP"};
    int ai, gi, pi, trial;

    if (argc > 1)
        numThreads = atoi(argv[1]);

    mkdir(baseDir.c_str(), 0755);

    for (pi = 0; pi < 2; pi++) {
        for (ai = 0; ai < 3; ai++) {
            for (gi = 0; gi < 3; gi++) {
                char dirname[256];
                snprintf(dirname, sizeof(dirname), "%s/%s_a%.1f_g%.1f",
                         baseDir.c_str(), policies[pi].c_str(),
                         alphas[ai], gammas[gi]);
                mkdir(dirname, 0755);

                for (trial = 0; trial < numTrials; trial++) {
                    Job job;
                    job.simFile = simFile;
                    job.outDir = string(dirname);
                    job.episodeLength = episodeLength;
                    job.seed = trial;
                    job.alpha = alphas[ai];
                    job.gamma_val = gammas[gi];
                    job.policy = policies[pi];
                    jobQueue.push(job);
                }
            }
        }
    }

    jobs_total = jobQueue.size();
    cout << "Running " << jobs_total << " ablation trials across "
         << numThreads << " threads on " << simFile << endl;

    vector<thread> threads;
    int t;
    for (t = 0; t < numThreads; t++)
        threads.push_back(thread(worker));
    for (t = 0; t < numThreads; t++)
        threads[t].join();

    cout << "Done. Data in " << baseDir << "/" << endl;
    return 0;
}
